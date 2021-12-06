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
	string git_dir = working_dir + "/.git";
	vector<string> exceptions;
	if(filesystem::is_directory(filesystem::status(git_dir)))
	{
		exceptions.push_back(git_dir);
		for(auto& gfile : filesystem::recursive_directory_iterator(git_dir))
		{
			exceptions.push_back(gfile.path());
		}
	}
	string 	input_file_string;// = argv[1];
	string	output_file_string = "list.todo";


	
	ifstream e_file;
	string estring;
	ifstream E_file;
	string Estring;
	vector<string> directory_vector;
	
	//TODO!! add exception files
	
	
	while((opt = getopt(argc, argv, "o:e:lL:")) != -1)
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
			case 'e':
				e_file.open(optarg);
				Estring = working_dir+ "/" + optarg;
				if(!filesystem::exists(filesystem::status(Estring)))
				{
					cout << "Exception file: \" " << Estring << "\" does not exist!" << endl;
					return -1;
				}
				exceptions.push_back(Estring);
				
				while(getline(e_file, estring))
				{
					if(filesystem::exists(filesystem::status(estring)))
					{
						exceptions.push_back(estring);
						if(filesystem::is_directory(filesystem::status(estring)))
						{	
							
							for(auto& Efile : filesystem::recursive_directory_iterator(estring))
							{
								exceptions.push_back(Efile.path());
							}
						}
						else
						{
							exceptions.push_back(estring);
						}
					}
				}
				e_file.close();
				break;
			case 'l':
				cout << "ToDo full file path tool" << endl;
				cout << "Printing all of the full file paths from your directory..." << endl;
				for(const auto& lfile : filesystem::directory_iterator(working_dir))
				{
					cout << "-> " <<lfile << endl;
				}
				return 0;
			case 'L':
				cout << "ToDo full file path tool" << endl;
				cout << "Printing all of the full file paths from specified directory..." << endl;
				for(const auto& lfile : filesystem::directory_iterator(optarg))
				{
					cout << "-> " << lfile << endl;
				}
				return 0;

		}
	}
	
	for(auto& file : filesystem::recursive_directory_iterator(working_dir))
	{
		if(!(find(exceptions.begin(), exceptions.end(), file.path().string()) != exceptions.end()))
		{
			directory_vector.push_back(file.path());
		}
	}
	
	if(directory_vector.size() < 1)
	{
		cout << error_message("Empty directory or no good items!") << endl;
		return -1;
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
	cout << "Start scan of directory!: " << working_dir << endl;
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
	output_file << working_dir << endl;


	output_file << endl << endl;

	output_file << "High Priority ToDos: " << endl;
	print_vector(high_output_vector, output_file);
	output_file << endl;

	output_file << "Mid Priority Todos: " << endl;
	print_vector(mid_output_vector, output_file);
	output_file << endl;
	
	output_file << "Low Priority Todos: " << endl;
	print_vector(low_output_vector, output_file);
	output_file << endl << endl;
	
	output_file << "Scanned directories and files:" << endl;
	for(auto each : directory_vector)
	{
		output_file << each << endl;
	}
	output_file << endl;

	output_file << "Excluded directories and files: " << endl;
	for(auto each2 : exceptions)
	{
		output_file << each2 << endl;
	}

	output_file.close();

	return 0;
}
