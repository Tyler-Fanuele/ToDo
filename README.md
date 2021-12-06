# The ToDo Tool

By Tyler Fanuele

## About

The ToDo tool lets the user plan their linux workflow by adding comments to their 
existing code and making a todo list for unfinished tasks.

## Adding the ToDo tool to your Path

### Linux:

1. Run ***"make && sudo ./setup"***  in console to add the program to the Path.
2. See the Using the Todo tool section for instructions on program comments.
3. Run the program with command ***"todo"*** to make a todo list of your current path.

## Using the ToDo tool

1. ToDo comments:
	1. Comment "//TODO! 'user message'" for a low priority todo.
	2. Comment "//TODO!! 'user message'" for a mid priority todo.
	3. Comment "//TODO!!! 'user message'" for a high priority todo.
2. Create a .todo file:
	1. This file will allow you to specify the place you would like your todo list to be output to.
3. Running the program:
	1. Type "todo" into the command line to create a todo list from your current path!

## Examples

- $ todo 
	- This example would produce a todo list from your current directory with the default output file name.
- $ todo -o output.todo
	- This example would produce a todo list from your current directory with "output.todo" as the output file.
- $ todo -i /foo
	- This example would produce a todo list from the directory /foo 
	and output to the default file.
- $ todo -o output.todo -e exceptions.txt
	- This example would produce a todo list from your current directory with a "output.todo" as the output file.
	The program will also check a list of exceptions listed in the exceptions.txt file and exclude them from your todo list.
- $ todo -l
	- This example will print you the full paths of all of the files in your current directory. You can use the -i command to 
	specify a different directory if you want.

## Commands

### -o "file path"

#### Output file setter

The -o command will set the file to output the todo file to.

#### Example:

$ todo -o output.txt

### -i "directory path"

#### Input path setter

This command will tell ToDo what path to scan to make a todo list.

#### Example:

$ todo -i /foo/bar

### -e "file path"

#### Exception flag

This command will pass a file to ToDo that will tell it what files to ignore in the current directory.
See the exceptions section for more information on a exception file.

#### Example:

$ todo -e exceptions.txt

### -l

#### Full path printer command

This command lets the user print the full paths of all of the files in the current directory. This can help the user
write the exception file. While you can use -i to specify a different directory for -l, it is recommended to use -L 
"FILE NAME" instead.

#### Example:
$ todo -l

### -L "directory path"

#### Full path printer command (with specified dir path)

This command lets the user print the full paths of all of the files in a argument specified directory.

#### Example:

$ todo -L /bar/foo

## Exceptions

### How to make a exception file

1. To make a exception file create a file with a name of your choosing, ex.txt for example
   Then figure out the full paths of the files you want to exclude. ToDo include -l and -L to
   make finding these easier as ToDo sees file paths exactly how -l and -L see them. 

2. Once the full file paths are known list them one by one in your exceptions file. Make sure they are exactly as
   They appear on the computer or they wont be excluded by ToDo when the program is called. Please use -l or -L
   if you are having trouble. Once your file is finished you are ready to use the -e command. See the exceptions example
   section for a exception file example


### Exception file example:

/usr/foo/bar.txt  
/usr/foo/lar.cpp  
/usr/foo/kar.cpp  
/usr/foo/lak.c  
/usr/foo/p.h  
 

