# ToDo Tool

## About
The ToDo tool lets the user plan their linux workflow by adding comments to their 
existing code and making a todo list for unfinished tasks.

## Adding the ToDo tool to your Path
### Linux:
1. Run ***"make && sudo ./setup"***  in console to add the program to the Path.
2. See the [Using Todo Tool]{: Using-ToDo-tool} tool section for instructions on program comments.
3. Run the program with command ***"todo"*** to make a todo list of your current path.

## Using ToDo tool
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
	- This example would produce a todo list with the default output file name.
- $ todo -o output.todo
	- This example would produce a todo list with "output.todo" as the output file.
