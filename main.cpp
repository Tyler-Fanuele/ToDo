#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <ctime>
#include <experimental/filesystem>
#include <string>

using namespace std;

#define G (32)
#define R (31)
#define B (34)
#define BLD (1)
#define UND (4)
#define REG (0)

string
add_color(string wString, int color, int formatting)
{
	string temp1 = "\033[" + to_string(formatting) + ";" + to_string(color) + "m";
	temp1 += wString + "\033[0m";
	return temp1;
}

string
pad(int len, char c)
{
	string ret = "";
	if (len < 0)
	{
		return "";
	}
	for (int i = 0; i < len; i++)
	{
		ret += c;
	}
	return ret;
}

string
error_message(string message)
{
	string ret_string = "|ERROR| " + message;
	return ret_string;
}

void add_to_vector(vector<pair<pair<string, int>, pair<string, int>>> &output_vector, string working_string, int line, string file_name, int demand)
{
	string temp = "";
	string temp2 = "";

	for (size_t i = 2; i < working_string.size(); i++)
	{
		temp += working_string[i];
	}
	temp2 += "|From File: " + file_name + " | At line: " + to_string(line);
	output_vector.push_back(pair<pair<string, int>, pair<string, int>>(pair<string, int>(temp, demand), pair<string, int>(file_name, line)));
}

void print_vector(vector<pair<pair<string, int>, pair<string, int>>> &output_vector, ostream &out)
{
	for (auto each : output_vector)
	{
		if (each.first.second == 0)
		{
			out << "[x] -> " << each.first.first << endl;
			out << "       From File: " << each.second.first << endl;
			out << "       From line: " << each.second.second << endl;
			out << endl;
		}
		else
		{
			out << "[ ] -> " << each.first.first << endl;
			out << "       From File: " << each.second.first << endl;
			out << "       From Line: " << each.second.second << endl;
			out << endl;
		}
	}
}

void print_help()
{
	cout << "ToDo list tool" << endl;
	cout << "By Tyler Fanuele" << endl;
	cout << endl
		 << "-o \"filename\" -> tells ToDo where to output." << endl;
	;
	cout << endl
		 << "-e \"filename\" -> tells ToDo where in your directory" << endl
		 << "                 the exception file is." << endl;
	cout << endl
		 << "-l            -> lists the full paths of every file in your current direcory." << endl;
	cout << endl
		 << "-L \"filename\" -> lists the full paths of every file in the argument specified "
		 << "directory." << endl;
}

int main(int argc, char **argv)
{

	// entry point, start getopt
	time_t now = time(NULL);
	string dnow = ctime(&now);
	int opt;
	string working_dir = filesystem::current_path();
	string git_dir = working_dir + "/.git";
	vector<string> exceptions;
	if (filesystem::is_directory(filesystem::status(git_dir)))
	{
		exceptions.push_back(git_dir);
		for (auto &gfile : filesystem::recursive_directory_iterator(git_dir))
		{
			exceptions.push_back(gfile.path());
		}
	}
	string input_file_string; // = argv[1];
	string output_file_string = "list.todo";

	ifstream r_file;

	ifstream e_file;
	string estring;
	ifstream E_file;
	string Estring;
	vector<string> directory_vector;

	// DONE!! add exception files

	while ((opt = getopt(argc, argv, "o:e:lL:h")) != -1)
	{
		switch (opt)
		{
		case '?':
			cout << error_message("Unknown option given") << endl;
			return -1;
			break;
		case 'h':
			print_help();
			return 0;
		case 'o':
			if (optarg == input_file_string)
			{
				cout << error_message("Output file string is the same as the input file string!") << endl;
				return -1;
			}
			output_file_string = optarg;
			break;
		case 'e':
			e_file.open(optarg);
			Estring = working_dir + "/" + optarg;
			if (!filesystem::exists(filesystem::status(Estring)))
			{
				cout << "Exception file: \" " << Estring << "\" does not exist!" << endl;
				return -1;
			}
			exceptions.push_back(Estring);

			while (getline(e_file, estring))
			{
				if (filesystem::exists(filesystem::status(estring)))
				{
					exceptions.push_back(estring);
					if (filesystem::is_directory(filesystem::status(estring)))
					{

						for (auto &Efile : filesystem::recursive_directory_iterator(estring))
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
			for (const auto &lfile : filesystem::directory_iterator(working_dir))
			{
				cout << "->" << lfile.path() << endl;
			}
			return 0;
		case 'L':
			cout << "ToDo full file path tool" << endl;
			cout << "Printing all of the full file paths from specified directory..." << endl;
			for (const auto &lfile : filesystem::directory_iterator(optarg))
			{
				cout << "-> " << lfile.path() << endl;
			}
			return 0;
		}
	}

	for (auto &file : filesystem::recursive_directory_iterator(working_dir))
	{
		if (!(find(exceptions.begin(), exceptions.end(), file.path().string()) != exceptions.end()))
		{
			directory_vector.push_back(file.path());
		}
	}

	if (directory_vector.size() < 1)
	{
		cout << error_message("Empty directory or no good items!") << endl;
		return -1;
	}

	cout << add_color("=== A todo list tool by Tyler Fanuele", G, UND) << endl;
	cout << add_color("=== Operational directory: ", G, REG)
		 << add_color(working_dir, G, REG) << endl;
	cout << add_color("===", G, REG) << endl;

	// end getops, start reading files
	ifstream input_file;
	ofstream output_file;
	string working_string;
	vector<pair<pair<string, int>, pair<string, int>>> low_output_vector;
	vector<pair<pair<string, int>, pair<string, int>>> mid_output_vector;
	vector<pair<pair<string, int>, pair<string, int>>> high_output_vector;
	output_file.open(output_file_string);

	for (auto working_file : directory_vector)
	{
		input_file_string = working_file;
		cout << add_color("=== Scanning ", G, UND) << add_color(input_file_string, B, UND) << endl;
		input_file.open(input_file_string);
		// output_file.open(output_file_string);

		int line = 1;
		int num_found = 0;
		while (getline(input_file, working_string))
		{
			string temp = "";
			string temp2 = "";
			working_string.erase(remove(working_string.begin(), working_string.end(), '\t'), working_string.end());
			if (working_string.rfind("//TODO!!! ", 0) == 0)
			{
				num_found++;
				add_to_vector(high_output_vector, working_string, line, input_file_string, 1);
				cout << add_color("===", G, REG) << pad(3, ' ') << " Saw TODO!!! token in "
					 << working_file << " at line: " << line << endl;
			}
			else if (working_string.rfind("//TODO!! ", 0) == 0)
			{
				num_found++;
				add_to_vector(mid_output_vector, working_string, line, input_file_string, 1);
				cout << add_color("===", G, REG) << pad(3, ' ') << " Saw TODO!! token in "
					 << working_file << " at line: " << line << endl;
			}
			else if (working_string.rfind("//TODO! ", 0) == 0)
			{
				num_found++;
				add_to_vector(low_output_vector, working_string, line, input_file_string, 1);
				cout << add_color("===", G, REG) << pad(3, ' ') << " Saw TODO! token in  "
					 << working_file << " at line: " << line << endl;
			}
			else if (working_string.rfind("//DONE!!! ", 0) == 0)
			{
				num_found++;
				add_to_vector(high_output_vector, working_string, line, input_file_string, 0);
				cout << add_color("===", G, REG) << pad(3, ' ') << " Saw DONE!!! token in "
					 << working_file << " at line: " << line << endl;
			}
			else if (working_string.rfind("//DONE!! ", 0) == 0)
			{
				num_found++;
				add_to_vector(mid_output_vector, working_string, line, input_file_string, 0);
				cout << add_color("===", G, REG) << pad(3, ' ') << " Saw DONE!! token in  " << working_file
					 << " at line: " << line << endl;
			}
			else if (working_string.rfind("//DONE! ", 0) == 0)
			{
				num_found++;
				add_to_vector(low_output_vector, working_string, line, input_file_string, 0);
				cout << add_color("===", G, REG) << pad(3, ' ') << " Saw DONE! token in  "
					 << working_file << " at line: " << line << endl;
			}
			line++;
		}
		if (num_found > 0)
		{
			cout << add_color("===", G, REG) << pad(3, ' ') << " Finished scanning " << working_file
				 << ": " << num_found << " tokens found" << endl;
		}
		else
		{
			cout << add_color("===", G, REG) << pad(3, ' ') << " No tokens found in " << working_file << endl;
		}
		// cout << "===" << endl;
		input_file.close();
		cout << add_color("===", G, REG) << endl;
	}
	cout << add_color("=== Directorys scanned successfully", G, REG) << endl;
	cout << add_color("=== Your list is located in ", G, UND) << add_color(output_file_string, B, UND) << endl;

	// End in console section of the program

	// start of output file printing

	const string BREAK = "++++++++++++++++++++++++++++++++++++++++++++++";

	// Print header
	output_file << "+++++ Todo list "
				<< "+++++" << endl;
	output_file << pad(7, ' ') << "Working Dir: " << working_dir << endl;
	output_file << pad(7, ' ') << "Last Todo list update: " << dnow;
	output_file << pad(7, ' ') << "Command: ";
	for (int m = 0; m < argc; m++)
	{
		output_file << argv[m] << " ";
	}
	output_file << endl
				<< endl;

	// Print high priority task section
	output_file << BREAK << endl;
	output_file << "High Priority tasks: " << endl;
	output_file << BREAK << endl;
	if (high_output_vector.size() < 1)
	{
		output_file << pad(7, ' ') << "No tasks..." << endl;
	}
	print_vector(high_output_vector, output_file);
	output_file << endl;

	// Print mid priority task section
	output_file << BREAK << endl;
	output_file << "Mid Priority Todos: " << endl;
	output_file << BREAK << endl;
	if (mid_output_vector.size() < 1)
	{
		output_file << pad(7, ' ') << "No tasks..." << endl;
	}
	print_vector(mid_output_vector, output_file);
	output_file << endl;

	// Print low priority task section
	output_file << BREAK << endl;
	output_file << "Low Priority Todos: " << endl;
	output_file << BREAK << endl;
	if (low_output_vector.size() < 1)
	{
		output_file << pad(7, ' ') << "No tasks..." << endl;
	}
	print_vector(low_output_vector, output_file);
	output_file << endl
				<< endl;

	// Print scanned directories and files
	output_file << BREAK << endl;
	output_file << "Scanned directories and files:" << endl;
	output_file << BREAK << endl;
	for (auto each : directory_vector)
	{
		output_file << pad(7, ' ') << each << endl;
	}
	output_file << endl;

	// Print excluded directories and files
	output_file << BREAK << endl;
	output_file << "Excluded directories and files: " << endl;
	output_file << BREAK << endl;
	for (auto each2 : exceptions)
	{
		output_file << pad(7, ' ') << each2 << endl;
	}

	output_file.close();
	// End output file printing

	return 0;
}
