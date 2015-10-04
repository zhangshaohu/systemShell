# systemShell
C program to implement a shell in Linux system
This C program is to implement a simple shell which has some functions as shown below:

•	display the prompt <Zhang> before command line;

•	read a command and its argument(s);

•	exit the shell;

•	continue reading the next command. 

Functions:
bool shell_cd(char **args)                            //function to valid "cd" command

int shell_exit(char **args)                           // function to valid "exit" command

char *read_command_line(void){             //function to read user command

char **split_command_line(char *line)    //function to split command line to character

int execute_command(char **args)          //function to execute a user command

To run this program:
gcc symbolTable.c –o hw1
./hw1
To exit the shell, please type ‘exit ‘.
This program can accept other Linux command, for example, type ‘ls’, the shell will  print file list in the existing folder.
