/*
    Copyright 2022 Tyler Fanuele
*/
#include <unistd.h>

#include <algorithm>
#include <ctime>
#include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define G (32)
#define R (31)
#define B (34)
#define BLD (1)
#define UND (4)
#define REG (0)

// Allows for formatting of string with color an format
string add_color(string wString, int color, int formatting) {
    string temp1 =
        "\033[" + to_string(formatting) + ";" + to_string(color) + "m";
    temp1 += wString + "\033[0m";
    return temp1;
}

// pads to len length of type c char
string pad(int len, char c) {
    string ret = "";
    if (len < 0) {
        return "";
    }
    for (int i = 0; i < len; i++) {
        ret += c;
    }
    return ret;
}

// pads to the length of the working_string with c char
string pad(string working_string, char c) {
    string ret = "";
    for (size_t i = 0; i < working_string.length(); i++) {
        ret += c;
    }
    return ret;
}

// formatts the message string as a error message
string error_message(string message) {
    string ret_string = "|ERROR| " + message;
    return add_color(ret_string, R, BLD);
}

// adds the working string (line), line number, file name and demand level
// to the vector
void add_to_vector(
    vector<pair<pair<string, int>, pair<string, int>>> &output_vector,
    string working_string, int line, string file_name, int demand) {
    string temp = "";
    string temp2 = "";

    for (size_t i = 2; i < working_string.size(); i++) {
        temp += working_string[i];
    }
    temp2 += "|From File: " + file_name + " | At line: " + to_string(line);
    output_vector.push_back(pair<pair<string, int>, pair<string, int>>(
        pair<string, int>(temp, demand), pair<string, int>(file_name, line)));
}

// prints vector
void print_vector(
    vector<pair<pair<string, int>, pair<string, int>>> &output_vector,
    ostream &out) {
    for (auto each : output_vector) {
        if (each.first.second == 0) {
            out << "[x] -> " << each.first.first << endl;
            out << "       From File: " << each.second.first << endl;
            out << "       From line: " << each.second.second << endl;
            out << endl;
        } else {
            out << "[ ] -> " << each.first.first << endl;
            out << "       From File: " << each.second.first << endl;
            out << "       From Line: " << each.second.second << endl;
            out << endl;
        }
    }
}

// prints the help command (-h)
void print_help() {
    cout << "ToDo list tool" << endl;
    cout << "By Tyler Fanuele" << endl;
    cout << endl << "-o \"filename\" -> tells ToDo where to output." << endl;
    cout << endl
         << "-e \"filename\" -> tells ToDo where in your directory" << endl
         << "                 the exception file is." << endl;
    cout << endl
         << "-l            -> lists the full paths of every file in your "
            "current direcory."
         << endl;
    cout << endl
         << "-L \"filename\" -> lists the full paths of every file in the "
            "argument specified "
         << "directory." << endl;
}

int main(int argc, char **argv) {
    // entry point, start getopt
    time_t now = time(NULL);
    string dnow = ctime(&now);
    int opt;
    string working_dir = filesystem::current_path();
    string git_dir = working_dir + "/.git";
    vector<string> exceptions;

    // add .git files to exceptions

    if (filesystem::is_directory(filesystem::status(git_dir))) {
        exceptions.push_back(git_dir);
        for (auto &gfile : filesystem::recursive_directory_iterator(git_dir)) {
            exceptions.push_back(gfile.path());
        }
    }

    string input_file_string;
    // set default output file name
    string output_file_string = "list.todo";

    ifstream r_file;

    ifstream e_file;
    string estring;
    ifstream E_file;
    string Estring;
    // sets default exceptions file name
    string exception_file_string = working_dir + "/" + ".exceptions";
    vector<string> directory_vector;

    /*
    mascott image
           __
      (___()'`;
      /,    /`
jgs   \\"--\\
    */
    // prints title screen
    cout << add_color("===           __", G, REG) << endl;
    cout << add_color("===      (___()'`;", G, REG) << endl;
    cout << add_color("===      /,    /`", G, REG) << endl;
    cout << add_color("===      \\\\'--\\\\", G, REG) << endl;
    cout << add_color("=== A todo list tool by Tyler Fanuele", G, UND) << endl;
    cout << add_color("=== Operational directory: ", G, REG)
         << add_color(working_dir, G, REG) << endl;
    cout << add_color("===", G, REG) << endl;

    // adds all exceptions files from default file if there, continue if not

    if (filesystem::exists(filesystem::status(exception_file_string))) {
        cout << add_color("=== Exceptions: ", G, REG)
             << add_color(".exceptions found", B, REG) << endl;
        cout << add_color("===", G, REG) << endl;
        exceptions.push_back(exception_file_string);
        e_file.open(exception_file_string);
        while (getline(e_file, estring)) {
            if (filesystem::exists(filesystem::status(estring))) {
                exceptions.push_back(estring);
                if (filesystem::is_directory(filesystem::status(estring))) {
                    for (auto &Efile :
                         filesystem::recursive_directory_iterator(estring)) {
                        exceptions.push_back(Efile.path());
                    }
                } else {
                    exceptions.push_back(estring);
                }
            }
        }
        e_file.close();

    } else {
        cout << add_color("=== Exceptions: .exceptions not found", R, BLD)
             << endl;
    }

    // get options from command line.
    while ((opt = getopt(argc, argv, "o:e:lL:h")) != -1) {
        switch (opt) {
            case '?':  // unknown option
                cout << error_message("Unknown option given") << endl;
                return -1;
                break;
            case 'h':  // help function
                print_help();
                return 0;
            case 'o':  // change output file name
                if (optarg == input_file_string) {
                    cout << error_message(
                                "Output file string is the same as the input "
                                "file string!")
                         << endl;
                    return -1;
                }
                output_file_string = optarg;
                break;
            case 'e':  // add non default exceptions file
                exception_file_string = optarg;
                e_file.open(exception_file_string);
                Estring = working_dir + "/" + optarg;
                // check if given exceptions file exists
                if (!filesystem::exists(filesystem::status(Estring))) {
                    cout << "Exception file: \" " << Estring
                         << "\" does not exist!" << endl;
                    return -1;
                } else {
                    cout << add_color("=== Exceptions: ", G, REG)
                         << add_color("Exception file given", B, REG) << endl;
                    cout << add_color("===", G, REG) << endl;
                }
                exceptions.push_back(Estring);

                // add all lines to exceptions vector
                while (getline(e_file, estring)) {
                    if (filesystem::exists(filesystem::status(estring))) {
                        exceptions.push_back(estring);
                        if (filesystem::is_directory(
                                filesystem::status(estring))) {
                            for (auto &Efile :
                                 filesystem::recursive_directory_iterator(
                                     estring)) {
                                exceptions.push_back(Efile.path());
                            }
                        } else {
                            exceptions.push_back(estring);
                        }
                    }
                }
                e_file.close();
                break;
            case 'l':  // list tool
                cout << "ToDo full file path tool" << endl;
                cout << "Printing all of the full file paths from your "
                        "directory..."
                     << endl;
                for (const auto &lfile :
                     filesystem::directory_iterator(working_dir)) {
                    cout << "->" << lfile.path() << endl;
                }
                return 0;
            case 'L':  // list specific directory tool
                cout << "ToDo full file path tool" << endl;
                cout << "Printing all of the full file paths from specified "
                        "directory..."
                     << endl;
                for (const auto &lfile :
                     filesystem::directory_iterator(optarg)) {
                    cout << "-> " << lfile.path() << endl;
                }
                return 0;
        }
    }

    // if the path is not in the exceptions vetor, add to vector of working
    // paths
    for (auto &file : filesystem::recursive_directory_iterator(working_dir)) {
        if (!(find(exceptions.begin(), exceptions.end(),
                   file.path().string()) != exceptions.end()) &&
            !filesystem::is_directory(filesystem::status(file))) {
            directory_vector.push_back(file.path());
        }
    }

    // check if we have anything at all to look through
    if (directory_vector.size() < 1) {
        cout << error_message("Empty directory or no good items!") << endl;
        return -1;
    }

    // end getops, start reading files
    ifstream input_file;
    ofstream output_file;
    string working_string;
    vector<pair<pair<string, int>, pair<string, int>>> low_output_vector;
    vector<pair<pair<string, int>, pair<string, int>>> mid_output_vector;
    vector<pair<pair<string, int>, pair<string, int>>> high_output_vector;
    output_file.open(output_file_string);

    // loop through each path in vector
    for (auto working_file : directory_vector) {
        input_file_string = working_file;
        cout << add_color("=== Scanning ", G, UND)
             << add_color(input_file_string, B, UND) << endl;
        input_file.open(input_file_string);
        // output_file.open(output_file_string);

        int line = 1;
        int num_found = 0;
        // loop through each line in that file
        while (getline(input_file, working_string)) {
            string temp = "";
            string temp2 = "";
            // take out leading tabs
            working_string.erase(
                remove(working_string.begin(), working_string.end(), '\t'),
                working_string.end());

            //look for each token string in each line.
            // TODO!! make this easier with a function, its too big right now
            if (working_string.find("// TODO!!! ") != string::npos) {
                num_found++;
                add_to_vector(high_output_vector, working_string, line,
                              input_file_string, 1);

                cout << add_color("===", G, REG) << pad(3, ' ')
                     << " Saw TODO!!! token in " << working_file
                     << " at line: " << line << endl;
            } else if (working_string.find("// TODO!! ") != string::npos) {
                num_found++;
                add_to_vector(mid_output_vector, working_string, line,
                              input_file_string, 1);
                cout << add_color("===", G, REG) << pad(3, ' ')
                     << " Saw TODO!! token in " << working_file
                     << " at line: " << line << endl;
            } else if (working_string.find("// TODO! ") != string::npos) {
                num_found++;
                add_to_vector(low_output_vector, working_string, line,
                              input_file_string, 1);
                cout << add_color("===", G, REG) << pad(3, ' ')
                     << " Saw TODO! token in  " << working_file
                     << " at line: " << line << endl;
            } else if (working_string.find("// DONE!!! ") != string::npos) {
                num_found++;
                add_to_vector(high_output_vector, working_string, line,
                              input_file_string, 0);
                cout << add_color("===", G, REG) << pad(3, ' ')
                     << " Saw DONE!!! token in " << working_file
                     << " at line: " << line << endl;
            } else if (working_string.find("// DONE!! ") != string::npos) {
                num_found++;
                add_to_vector(mid_output_vector, working_string, line,
                              input_file_string, 0);
                cout << add_color("===", G, REG) << pad(3, ' ')
                     << " Saw DONE!! token in  " << working_file
                     << " at line: " << line << endl;
            } else if (working_string.find("// DONE! ") != string::npos) {
                num_found++;
                add_to_vector(low_output_vector, working_string, line,
                              input_file_string, 0);
                cout << add_color("===", G, REG) << pad(3, ' ')
                     << " Saw DONE! token in  " << working_file
                     << " at line: " << line << endl;
            }
            line++;
        }
        // print number found if > 0 and none if not
        if (num_found > 0) {
            cout << add_color("===", G, REG) << pad(3, ' ')
                 << " Finished scanning " << working_file << ": " << num_found
                 << " tokens found" << endl;
        } else {
            cout << add_color("===", G, REG) << pad(3, ' ')
                 << " No tokens found in " << working_file << endl;
        }
        // cout << "===" << endl;
        input_file.close();
        cout << add_color("===", G, REG) << endl;
    }
    // display closing text
    cout << add_color("=== Directorys scanned successfully", G, REG) << endl;
    cout << add_color("=== Your list is located in ", G, UND)
         << add_color(output_file_string, B, UND) << endl;

    // End in console section of the program

    // start of output file printing

    const string BREAK = "++++++++++++++++++++++++++++++++++++++++++++++";

    // Print header
    output_file << "+++++ Todo list "
                << "+++++" << endl;

    output_file << pad(7, ' ') << "Working Dir: " << working_dir << endl;

    output_file << pad(7, ' ') << "Last Todo list update: " << dnow;

    output_file << pad(7, ' ') << "Command: ";

    for (int m = 0; m < argc; m++) {
        output_file << argv[m] << " ";
    }
    output_file << endl << endl;

    // Print high priority task section
    output_file << BREAK << endl;

    output_file << "High Priority tasks: " << endl;

    output_file << BREAK << endl;

    if (high_output_vector.size() < 1) {
        output_file << pad(7, ' ') << "No tasks..." << endl;
    }

    print_vector(high_output_vector, output_file);
    output_file << endl;

    // Print mid priority task section
    output_file << BREAK << endl;

    output_file << "Mid Priority Todos: " << endl;

    output_file << BREAK << endl;

    if (mid_output_vector.size() < 1) {
        output_file << pad(7, ' ') << "No tasks..." << endl;
    }
    print_vector(mid_output_vector, output_file);
    output_file << endl;

    // Print low priority task section
    output_file << BREAK << endl;

    output_file << "Low Priority Todos: " << endl;

    output_file << BREAK << endl;

    if (low_output_vector.size() < 1) {
        output_file << pad(7, ' ') << "No tasks..." << endl;
    }
    print_vector(low_output_vector, output_file);
    output_file << endl << endl;

    // Print scanned directories and files
    output_file << BREAK << endl;

    output_file << "Scanned directories and files:" << endl;

    output_file << BREAK << endl;
    for (auto each : directory_vector) {
        output_file << pad(7, ' ') << each << endl;
    }
    output_file << endl;

    // Print excluded directories and files
    output_file << BREAK << endl;
    output_file << "Excluded directories and files: " << endl;
    output_file << BREAK << endl;
    for (auto each2 : exceptions) {
        output_file << pad(7, ' ') << each2 << endl;
    }

    output_file.close();
    // End output file printing

    return 0;
}
