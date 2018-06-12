/*
    Bruno Vedoveto Leandro
    Giovanna Cazelato Pires
    Wesley Otto
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define STR_MAX 255

void clear_input(char*);
int  create_process(char***, int);
void ignore_handler(int);
int  test_stdin(char*);
void replaceHomeDir(char*);
void signal_handler();
void typePrompt();
int create_process(char***, int);
int spawn_proc(int, int, char**);

void clean_up_child_process (int signal_number)
{
  /* Clean up the child process.  */
  int status;
  wait(&status);
}

int main() {
  char fullCommand[STR_MAX];
  //fullCommand[0] = ' ';
  int firstArgCount;
  signal_handler();

  while(1) {                            // Repeats forever
    // Reads and parser user input
    do{
      typePrompt();                       // Shows prompt on screen
      clear_input(fullCommand);           //Clear stdin

      // Reads and parser user input
      fgets(fullCommand, sizeof(char) * STR_MAX, stdin);
      if (fullCommand[strlen(fullCommand)-1] == '\n')
         fullCommand[strlen(fullCommand)-1] = '\0';

    }while(!(test_stdin(fullCommand)));

    // If the user types "exit" the shell is exited
    if (strcmp(fullCommand, "exit") == 0) exit(0);

    /* BEGIN -------------------------- building arg_list -------------------------- */
    // Counts pipes
    char *token_p;
    char tokenFullCommand[STR_MAX];
    strcpy(tokenFullCommand, fullCommand);
    int pipeCount = -1;
    for(token_p = strtok(tokenFullCommand, "|"); token_p != NULL; token_p = strtok(NULL, "|"))
      pipeCount++;

    // Allocate the arg_list for pipes
    char*** arg_list = (char***) malloc((pipeCount+1) * sizeof(char**));
    // Allocate the command array
    char** command = (char**) malloc((pipeCount+1) * sizeof(char*));

    int i_command = 0;
    strcpy(tokenFullCommand, fullCommand);
    for(token_p = strtok(tokenFullCommand, "|"); token_p != NULL; token_p = strtok(NULL, "|")) {
      command[i_command] = (char*) malloc(STR_MAX * sizeof(char));
      strcpy(command[i_command], token_p);
      i_command++;
    }

    i_command = 0;
    strcpy(tokenFullCommand, fullCommand);
    while(i_command <= pipeCount) {
      // Counts tokens
      char *token;
      char tokenCommand[STR_MAX];
      strcpy(tokenCommand, command[i_command]);
      int argCount = 0;
      for(token = strtok(tokenCommand, " "); token != NULL; token = strtok(NULL, " "))
        argCount++;

      if (i_command == 0) firstArgCount = argCount; // Store to use in "Change Dir" (CD) part

      // Allocate the arg_list for commands
      arg_list[i_command] = (char**) malloc((argCount+1) * sizeof(char*));

      // Copies the tokens values to arg_list
      int j_command = 0;
      strcpy(tokenCommand, command[i_command]);
      for(token = strtok(tokenCommand, " "); token != NULL; token = strtok(NULL, " ")) {
        // Allocate the arg_list for arguments
        arg_list[i_command][j_command] = (char*) malloc(STR_MAX * sizeof(char));
        strcpy(arg_list[i_command][j_command], token);
        j_command++;
      }
      arg_list[i_command][j_command] = NULL;
      i_command++;
    }
    /* END -------------------------- building arg_list -------------------------- */

    // Check for "Change Dir (CD)"
    if (strcmp(arg_list[0][0], "cd") == 0) {
       if (arg_list[0][1] == NULL) {
          char homeDir[50] = "";
          strcpy(homeDir, getenv("HOME"));

          arg_list[0][1] = (char*) malloc(STR_MAX * sizeof(char));
          strcpy(arg_list[0][1], homeDir);
       }

       if (chdir(arg_list[0][1]) != 0)
          fprintf(stderr, "cd: '%s' file or directory not found\n", arg_list[0][1]);

       continue;
    }

    int stdin_cp = STDIN_FILENO;
    int stdout_cp = STDOUT_FILENO;

    create_process(arg_list, pipeCount+1); // cmd count should be pipeCount+1 [Example: ls | cat ]. 1 pipe. 2 commands.
    wait(0);
    dup2(stdin_cp, STDIN_FILENO);
    dup2(stdout_cp, STDOUT_FILENO);

  } // END SHELL WHILE(1)

}

int create_process(char ***arg_list, int cmd_count)
{
  int command_i;
  pid_t pid;
  int in, out, pipe_fd[2];

  /* The first process should get its input from the original file descriptor STDIN_FILENO.  */
  in = STDIN_FILENO;

  /* Spawn all process here.  */
  for (command_i = 0; command_i < cmd_count; command_i++) {
    pipe(pipe_fd);

    /* The last process should write to the original file descriptor STDOUT_FILENO.  */
    if (command_i == cmd_count-1)
       out = STDOUT_FILENO;
    else
       out = pipe_fd[1];

    /* pipe_fd[1] is the write end of the pipe, we carry "in" from the previous iteration.  */
    if ( (pid = spawn_proc(in, out, arg_list[command_i])) < 0 ) {
       fprintf(stderr, "Command '%s' not found\n", arg_list[command_i][0]);
       exit(-1);
    }

    /* No need for the write end of the pipe, the child will write here.  */
    close(pipe_fd[1]);

    /* Keep the read end of the pipe, the next child will read from there.  */
    in = pipe_fd[0];
  }

  return pid;
}

int spawn_proc(int in, int out, char **command) {
  pid_t pid;

  if ((pid = fork()) == 0) {
    if (in != STDIN_FILENO) {
      dup2(in, STDIN_FILENO);
      close(in);
    }

    if (out != STDOUT_FILENO) {
      dup2(out, STDOUT_FILENO);
      close(out);
    }

    return execvp(command[0], command);
  }

  return pid;
}

void typePrompt(){
  //Initiate prompt
  fprintf(stdout, "[MySh] ");

  //Prompt user and hostname
  char username[50] = "";
  strcpy(username, getenv("USER"));
  char hostname[50] = "";
  gethostname(hostname, sizeof(hostname));
  fprintf(stdout, "%s@%s:", username, hostname);

  //Prompt current directory
  char currDir[STR_MAX] = "";
  getcwd(currDir, sizeof(currDir));
  replaceHomeDir(currDir);
  fprintf(stdout, "%s$ ", currDir);
}

void replaceHomeDir(char* currDir) {
  char homeDir[50] = "";
  strcpy(homeDir, getenv("HOME"));

  char tmpCurrDir[STR_MAX] = "";
  strcpy(tmpCurrDir, currDir);
  tmpCurrDir[strlen(homeDir)] = '\0';

  if (strcmp(tmpCurrDir, homeDir) == 0) {
    strncpy(tmpCurrDir, currDir + strlen(homeDir), strlen(currDir) - strlen(homeDir) + 1);
    strcpy(currDir, "~");
    strcat(currDir, tmpCurrDir);
  }
}

int test_stdin(char* fullCommand){
  if (feof(stdin)) {
    // CTRL-D case
    strcpy(fullCommand, "exit");
  } else if (ferror(stdin)) { // Clears the error indication flag and ^C, ^Z or any other unknown character
    clearerr(stdin);
    return 0;
  }
  return 1;
}

void signal_handler(){
  // CTRL + C SIGNAL
  struct sigaction sigint_action;
  memset(&sigint_action, 0, sizeof(sigint_action));
  sigint_action.sa_handler = &ignore_handler;
  sigaction(SIGINT, &sigint_action, NULL);

  // CTRL + Z SIGNAL
  struct sigaction sigstp_action;
  memset(&sigstp_action, 0, sizeof(sigstp_action));
  sigstp_action.sa_handler = SIG_IGN;
  sigaction(SIGTSTP, &sigstp_action, NULL);

  /* Handle SIGCHLD by calling clean_up_child_process.  */
  struct sigaction sigchld_action;
  memset (&sigchld_action, 0, sizeof(sigchld_action));
  sigchld_action.sa_handler = &clean_up_child_process;
  sigaction(SIGCHLD, &sigchld_action, NULL);
}

void ignore_handler(int signal_number){
  fprintf(stdout, "\n");
  return;
}

void clear_input(char* fullCommand) {
    fflush(stdin);
    fullCommand[0] = '\0';
}