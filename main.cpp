#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <algorithm>
#include <vector>

using namespace std;

string 
error_message(string message)
{
	string ret_string = "|ERROR| " + message;
	return ret_string;
}

void 
add_to_vector(vector<pair<pair<string, int>, pair<string, int>>>& output_vector, string working_string ,int line, string file_name, int demand)
{
	string temp = "";
	string temp2 = "";

	for(int i = 2; i < working_string.size(); i++)
	{
		temp += working_string[i];
	}
	temp2 += "|From File: " + file_name + " | At line: " + to_string(line);
	output_vector.push_back(pair(pair(temp, demand), pair(file_name, line)));
}

void print_vector(vector<pair<pair<string, int>, pair<string, int>>>& output_vector, ostream& out)
{
	for(auto each : output_vector)
	{
		out << "Message: " << each.first.first<< endl;
		out << "From File: " <<each.second.first;
		out << " From line: " << each.second.second << endl;
		out << endl;
	}
}

int main(int argc, char** argv)
{

	//entry point, start getopt
	int 	opt;
	string working_dir = filesystem::current_path();
	//cout << argc << endl;
	/*
	if(argc < 2)
	{
		cout << error_message("Not enough arguments!") << endl;
		cout << error_message("No input file given!") << endl;
		return -1;		
	}
	*/
	string 	input_file_string = argv[1];
	string  t = "";
	for(int j = 0; j < input_file_string.size(); j++)
	{
		if(input_file_string[j] == '.')
		{
			break;
		}
		t += input_file_string[j];
	}
	string	output_file_string = t + ".todo";

	//cout << input_file_string << endl;
	

	vector<string> directory_vector;
	
	//TODO!! add exception files

	for(const auto& file : filesystem::directory_iterator(working_dir))
	{
		directory_vector.push_back(file.path());
	}

	if(directory_vector.size() < 1)
	{
		cout << error_message("Empty directory or no good items!") << endl;
		return -1;
	}
	
	/*
	for(auto each : directory_vector)
	{
		cout << each << endl;
	}
	*/

	
	
	
	while((opt = getopt(argc, argv, "o:")) != -1)
	{
		switch(opt)
		{
			case '?':
				cout << error_message("Unknown option given") << endl;
				return -1;
				break;
			case 'o':
				if(optarg == input_file_string)
				{
					cout << error_message("Output file string is the same as the input file string!") << endl;
					return -1;
				}
				output_file_string = optarg;
				break;

		}
	}

	cout << "Todo Tool" << endl;
	cout << "By Tyler Fanuele" << endl;
	cout << "Working on " << working_dir << endl << endl;

	//end getops, start reading files
	ifstream input_file;
	ofstream output_file;
	string working_string;
	vector<pair<pair<string, int> ,pair<string, int>>> low_output_vector;
	vector<pair<pair<string, int> ,pair<string, int>>> mid_output_vector;
	vector<pair<pair<string, int> ,pair<string, int>>> high_output_vector;
	output_file.open(output_file_string);
	cout << "Start scan of directory!: " << endl;
	for(auto working_file : directory_vector)
	{
		input_file_string = working_file;
		cout << "Scanning " << input_file_string << "!" << endl;
		input_file.open(input_file_string);
		//output_file.open(output_file_string);
	
		int line = 1;
		while(getline(input_file, working_string))
		{
			string temp = "";
			string temp2 = "";
			working_string.erase(remove(working_string.begin(), working_string.end(), '\t'), working_string.end());
			if(working_string.rfind("//TODO!!!", 0) == 0)
			{
				add_to_vector(high_output_vector, working_string, line, input_file_string, 3);
			}	
			else if(working_string.rfind("//TODO!!", 0) == 0)
			{	
			
				add_to_vector(mid_output_vector, working_string, line, input_file_string, 2);
			}
			else if(working_string.rfind("//TODO!", 0) == 0)
			{
			
				add_to_vector(low_output_vector, working_string, line, input_file_string, 1);
			}
			line++;
		}
		input_file.close();
	}
	cout << "Finished directory scan!" << endl << endl;
	cout << "Todo list should be located in: " << output_file_string << endl;

	output_file << "TODO LIST FOR: " << endl;
	for(auto each : directory_vector)
	{
		output_file << each << endl;
	}
	output_file << endl << endl;

	output_file << "High Priority ToDos: " << endl;
	print_vector(high_output_vector, output_file);
	output_file << endl;

	output_file << "Mid Priority Todos: " << endl;
	print_vector(mid_output_vector, output_file);
	output_file << endl;
	
	output_file << "Low Priority Todos: " << endl;
	print_vector(low_output_vector, output_file);
	output_file << endl;

	output_file.close();

	return 0;
}
