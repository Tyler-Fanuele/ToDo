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
	cout << "|";
	temp2 += "|From File: " + file_name + " | At line: " + to_string(line);
	output_vector.push_back(pair(pair(temp, demand), pair(file_name, line)));
}

void print_vector(vector<pair<pair<string, int>, pair<string, int>>>& output_vector, ostream& out)
{
	for(auto each : output_vector)
	{
		out << each.first.first<< endl;
		out << each.first.second << endl;
		out << each.second.first << endl;
		out << each.second.second << endl;
		out << endl;
	}
}

int main(int argc, char** argv)
{

	//entry point, start getopt
	int 	opt;
	//cout << argc << endl;
	if(argc < 2)
	{
		cout << error_message("Not enough arguments!") << endl;
		cout << error_message("No input file given!") << endl;
		return -1;		
	}
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


	//end getops, start reading file
	ifstream input_file;
	ofstream output_file;
	string working_string;
	vector<pair<pair<string, int> ,pair<string, int>>> output_vector;
	
	input_file.open(input_file_string);
	output_file.open(output_file_string);

	int line = 1;
	while(getline(input_file, working_string))
	{
		string temp = "";
		string temp2 = "";
		working_string.erase(remove(working_string.begin(), working_string.end(), '\t'), working_string.end());
		if(working_string.rfind("//TODO!!!", 0) == 0)
		{
			add_to_vector(output_vector, working_string, line, input_file_string, 3);
		}
		else if(working_string.rfind("//TODO!!", 0) == 0)
		{	
			
			add_to_vector(output_vector, working_string, line, input_file_string, 2);
		}
		else if(working_string.rfind("//TODO!", 0) == 0)
		{
			
			add_to_vector(output_vector, working_string, line, input_file_string, 1);
		}
		line++;
	}

	print_vector(output_vector, output_file);
	
	output_file.close();
	input_file.close();

	return 0;
}
