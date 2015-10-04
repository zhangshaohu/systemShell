#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//===========================================
// Name: Shaohu Zhang
// Compiler used: gcc on Linux server
//
// This program implements a simple shell to:
// display the prompt <Zhang> before command line;
// read a command and its argument(s);
// exit the shell;
// continue reading the next command. 
//===========================================
typedef int bool;                                                       //define data type:boolean
enum { false, true };                                                   //false = 0, true = 1

                                                                        //function declarations
bool shell_cd(char **args);
int shell_exit(char **args);

char *shell_cmd[]={"cd","exit"};                                        //command list
int (*command_list[]) (char **) = { &shell_cd, &shell_exit};

bool shell_cd(char **args)                                              //function to valid "cd" command 
{                                                                                       
  if (args[1] == NULL) {                                                //if no command input,print error
    fprintf(stderr, "Error: expected argument to \"cd + name\"\n");
  } else {                                                              //if no space after cd,print error
    if (chdir(args[1]) != 0) {
      perror("Error");
    }
  }
  return true;                                        //otherwise, return true
}


int shell_exit(char **args)                           // function to valid "exit" command
{
  return 0;
  
}


#define CMD_SIZE 100                                  //define the size of command as 100 characters
char *read_command_line(void){                        //function to read user command
  int buf = CMD_SIZE;
  int pos = 0;
  char *buffer = malloc(sizeof(char) * buf);
  int ch;
  if (!buffer) {
    fprintf(stderr, "Error: allocation error\n");
    exit(EXIT_FAILURE);
  }
  while (true) {                                                 
    ch = getchar();                                  // Read a character
    if (ch == EOF || ch == '\n') {                   //if it hits EOF or newline, replace it as null
      buffer[pos] = '\0';
      return buffer;
    } else {
      buffer[pos] = ch;                              
    }
  pos++;
  }	
}

#define BUFSIZE 64                                  //define the size of buffer as 64 
#define CMD_DELIM " \t\r\n\a"                       //define the delimiters
char **split_command_line(char *line)               //function to split command line to character
{
  int buf = BUFSIZE, pos = 0;
  char **cmds = malloc(buf * sizeof(char*));
  char *cmd;
  if (!cmds) {
    fprintf(stderr, "Error: allocation error\n");
    exit(EXIT_FAILURE);
  }
  cmd = strtok(line, CMD_DELIM);
  while (cmd != NULL) {
    cmds[pos] = cmd;
    pos++;
    cmd = strtok(NULL, CMD_DELIM);
  }
  cmds[pos] = NULL;
  return cmds;
}

bool shell_launch(char **args)                     //function to create a process
{
  pid_t pid;
  int status;
  pid = fork();                                   //fork a child process
  if (pid<0){
	  perror("Error");                            //Error fork
  }
  else if (pid == 0) {                            //Child process
    if (execvp(args[0], args) == -1) {
      perror("Error");
    }
    exit(EXIT_FAILURE);
  }  else {
                                                  // Parent process will wait for the child to complete
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return true;
}

int execute_command(char **args)                  //function to execute a user command
{
  int i;
  if (args[0] == NULL) {                          // If command is empty.
   return 1;
  }

  for (i = 0; i <(sizeof(shell_cmd)/sizeof(char*)); i++) {
    if (strcmp(args[0], shell_cmd[i]) == 0) {    //compare input command with given command list
      return (*command_list[i])(args);
	 
    }
  }
  return shell_launch(args);
}

int main(void)
{
 
  char *command_line;
  char **command_args;
  int status;

                                                          // Run loop.
  do {
    printf("<Zhang> ");
    command_line = read_command_line();
    command_args = split_command_line(command_line);
    status = execute_command(command_args);
                                                         //deallocate memory 
    free(command_line);
    free(command_args);
  } while (status);
  return 0;
}

