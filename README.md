# The ToDo Tool

> By Tyler Fanuele

## About

The ToDo tool lets the user plan their linux / mac workflow by adding comments to their
existing code and making a todo list for unfinished tasks. This tool is meant to work with
any programming language that allows for `//` comments. Git files are ignored by default.

> Now with support for more languages!

## Possible ways to use ToDo list tool

ToDo tool is designed to be ran every time you add or change a TODO comment, this way your
todo list is always up to date with your tasks. Todo can also be added to a Makefile so that
when your project is built the todo list will also be updated.

## Adding the ToDo tool to your Path

### Linux

1. Run `make` and `sudo ./setup` in console to add the program to the Path.
2. See the Using the Todo tool section for instructions on program comments.
3. Run the program with command `todo` to make a todo list of your current path.

### Mac

1. Add the file path to your `/ect/paths` folder and restart terminal.
2. Run `make`.
3. Then you can run the program with the command `todo`.

## Using the ToDo tool

1. ToDo comments:
    1. Comment `TODO! 'user message'` for a low priority task.
    2. Comment `TODO!! 'user message'` for a mid priority task.
    3. Comment `TODO!!! 'user message'` for a high priority task.
2. Done comments:
    - Change the `TODO!` comment to `DONE!` to check off the task.
   This works for all levels of tasks.
3. Create a .todo file:
    - This file will allow you to specify the place you would like your todo list to be output to.
4. Optionally create a exception file:
    - See the exceptions section for more details.
5. Running the program:
    - Type `todo` into the command line to create a todo list from your current path!

| Command | Description | Example|
| ------- | ----------- | ---------------------- |
| `-o "file path"` | The -o command will set the file to output the todo file to | `$ todo -o output.txt` |
| `-e "file path"` | This command will pass a file to ToDo that will tell it what files or directories to ignore in the current directory. | `$ todo -e exceptions.txt` |
| `-l` | This command lets the user print the full paths of all of the files in the current directory. | `$ todo -l` |
|`-L "directory path"` | This command lets the user print the full paths of all of the files in a argument specified directory. | `$ todo -L /bar/foo` |

## Examples

- `$ todo`
  - This example would produce a todo list from your current directory with the default output file name.
- `$ todo -o output.todo`
  - This example would produce a todo list from your current directory with "output.todo" as the output file.
- `$ todo -o output.todo -e exceptions.txt`
  - This example would produce a todo list from your current directory with a "output.todo" as the output file.
 The program will also check a list of exceptions listed in the exceptions.txt file and exclude them from your todo list.
- `$ todo -l`
  - This example will print you the full paths of all of the files in your current directory.
- `$ todo -L /foo/bar`
  - This example will print the full paths of all the things in the argument passed file path.

## Exceptions

### How to make a exception file

By default the program will look for an exception file called '.exceptions'. If this is not found the program will either run
without exceptions or use the user defined exceptions file with 'e'.

1. To make a exception file create a file with a name of your choosing, ex.txt for example
   Then figure out the full paths of the files or directories you want to exclude. ToDo include `-l` and `-L` to
   make finding these easier as ToDo sees file paths exactly how `-l` and `-L` see them.

2. Once the full paths are known list them one by one in your exceptions file. Make sure they are exactly as
   They appear on the computer or they wont be excluded by ToDo when the program is called. Please use `-l` or `-L`
   if you are having trouble. Once your file is finished you are ready to use the `-e` command. See the exceptions example
   section for a exception file example.

### Exception file example

/usr/foo/bar.txt  
/usr/foo/lar.cpp  
/usr/foo/kar.cpp  
/usr/foo/lak.c  
/usr/foo/p.h  
/usr/foo/bar
