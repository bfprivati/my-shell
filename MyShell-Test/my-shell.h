#define MAX_ARR_SIZE 1024
#define MAX_CMD_SIZE 1024
#define ANSI_ESCAPE_SEQUENCE(EscapeSeq)   "\33[" EscapeSeq
#define CTRLL 0xOC
#define in 0
#define out 1

int redir_out = 0; /* Se (>), a variável redir_out será setada para 1    */
int redir_in = 0;  /* Se (<), a variável redir_in será setada para 1     */
int redir_err = 0; /* Se (2>), a variável redir_err será setada para 1   */


void show_prompt(){
    int i;
    char * user;
    char host[MAX_ARR_SIZE];
    char cwdir[MAX_ARR_SIZE];
    char * home;
    char * token;

    user = getenv("USER");
    home = getenv("HOME");
    gethostname(host, MAX_ARR_SIZE);
    getcwd(cwdir, sizeof(cwdir));


    char * params[MAX_ARR_SIZE];
    token = strtok(cwdir, "/");

    i = 0;
    params[i] = (char *) malloc(sizeof(strlen(token)));
    strcpy(params[i], token);
    token = strtok(NULL, "/");

    if( strcmp(params[i], "home") == 0 )
        params[i] = "~";

    printf("%s@%s %s", user, host, params[0]);
    i++;

    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        token = strtok(NULL, "/");
        if ( strcmp(params[i], user) == 0 )
            params[i] = "";

        printf("%s/", params[i]);
        i++;
    }
        printf(" $ ");
    return;
}

void tratar_signal (int signum){
	printf("\n");
}

void signal_handler (){
    signal(SIGHUP, SIG_IGN);    // bloquear KILL
    signal(SIGINT, SIG_IGN);    // bloquear CTRLC
    signal(SIGTSTP, SIG_IGN);   // bloquear CTRLZ

	struct sigaction new_action; //struct da System Call Sigaction
	memset(&new_action, 0, sizeof(new_action)); //inicializar a variavel
	new_action.sa_handler = &tratar_signal; //recebe nova acao

	sigaction(SIGINT, &new_action, NULL); 	//CTRL+C
	sigaction(SIGTSTP, &new_action, NULL); 	//CTRL+Z
    	//ENTER
}

void io_rdrct(char *entrada, char *saida, char *error){

    int fdin = open(entrada, O_RDONLY, 0);
    int fdout = open(saida, O_WRONLY|O_APPEND, 0);
    int fderror = open(error, O_WRONLY, 0);

    dup2(fdin, 0);
    dup2(fdout, 1);
    dup2(fderror, 2);

    close(fdin);
    close(fdout);
    close(fderror);

    return;
}

int create_process(char **params, char *arquivo){
    int fd, error;
    int pid = fork();

    if (pid == 0) {
    // codigo do processo filho
        if(redir_in == 1 && arquivo != NULL){
            fd = open(arquivo, O_WRONLY | O_CREAT, 0777);
            dup2(fd, 0);
        }
        else if(redir_out == 1 && arquivo != NULL){
            fd = open(arquivo, O_WRONLY | O_CREAT, 0777);
            dup2(fd, 1);
        }
        else if(redir_err == 1){
            fd = open(arquivo, O_WRONLY | O_CREAT, 0777);
            dup2(fd, 2);
        }

        execvp(params[0], params);
        //error = execvp(params[0], params);
        close(fd);

        if(error != NULL){
            printf("%s\n", strerror(errno));
        }
        return 0;

    } else {
    // codigo do processo pai
        int res;
        waitpid(pid, &res, 0);

        if( redir_out == 1 || redir_in == 1 || redir_err == 1 ){
            redir_out = 0;
            redir_in = 0;
            redir_err = 0;
        }
        
        if (WIFEXITED(res) && (WEXITSTATUS(res)==0))
            return 1;
    }

    return 1;
}

int read_command() {
    int i=0, j=0, dir;
    char cwdir[MAX_ARR_SIZE];
    char command[MAX_CMD_SIZE];
    char * token;
    char * params[MAX_ARR_SIZE];
    char * arquivo = NULL;

    //Zerar string
	for (j=0; j< MAX_CMD_SIZE; j++){
		command[j] = '\0';
	}
	for (j=0; j< MAX_ARR_SIZE; j++){
		params[j] = '\0';
	}

    // ler comando e tirar espaços
    fflush(stdin);
    fgets(command, sizeof(char) * MAX_ARR_SIZE, stdin );
    if(command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';
    fflush(stdin);
    token = strtok(command, " ");

    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        token = strtok(NULL, " ");

        if ( strcmp(params[i], "exit") == 0 ){
        // Sair do terminal OK

            return -2;
        } 
        else if (strcmp(params[i], "cd") == 0){
        // Mover entre diretórios OK

            if ((token == NULL) || (token == "~")){
            	dir = chdir(getenv("HOME"));
            	if(dir != 0)//se retornar -1, ocorreu erro
                        fprintf(stderr, "cd: '%s' file or directory not found\n", params[i]);
            } else {
	            while(token != NULL) {
	                dir = 0;
	                params[i] = (char *) malloc(sizeof(strlen(token)));//aloca...
	                strcpy(params[i], token);//Copia token
	                token = strtok(NULL, " ");

	                dir = chdir(params[i]);
                    if(dir != 0)//se retornar -1, ocorreu erro
	                    fprintf(stderr, "cd: '%s' file or directory not found\n", params[i]);

	                i++;
	            }
            }

            for (j=0; j< MAX_CMD_SIZE; j++){
    			command[j] = '\0';
                params[j] = '\0';
			}
        }  
        else if ( (strcmp(params[i], ">") == 0) || (strcmp(params[i], "<") == 0) || (strcmp(params[i], "2>") == 0)) {
        // Redirecionar arquivos
            printf("ENTROU AQUI\n\n");
            
            if((strcmp(params[i], ">") == 0)){
                redir_out = 1;
            } 
            else if ((strcmp(params[i-1], "<") == 0)){
                redir_in = 1;
            } 
            else if ((strcmp(params[i], "2>") == 0)) {
                redir_err = 1;
            }

            i++;
            params[i] = (char *) malloc(sizeof(strlen(token)));
            strcpy(params[i], token);
            token = strtok(NULL, " ");

            printf("parametro %d: %s", i, params[i]);
            arquivo = (char *) malloc(sizeof(strlen(params[i])));
            strcpy(arquivo, params[i]);
    
            params[i-1] = NULL;
        }

        i++;
    }
    printf("****VEIO AQUI****\n\n");
    create_process(params, arquivo);

    return 1;
}




/* --------------------------------------------------------------------------------------------------------------------------*/



    // SE TEM PIPE, RETORNA ARRAY DE ARRAYS E USAR FORK OUTRO spawn_process(params, sizeof(params)/8);

// int spawn_process(char **params, int len) {
//     // SOMENTE USAR QUANDO LS -LA | SORT | MORE
//     // USAR ARRAY DE ARRAYS, TRATANDO |
//
//     int i;
//     int fd[2];
//     int pid;
//     for(i=0; i < len -1 ; i++) {
//         pipe(fd);
//         pid = fork();
//         if (pid == 0) {
//             // filho
//             dup2(fd[out],out);
//             close(fd[out]);
//             close(fd[in]);
//             execvp(params[i][0],params[i]);
//         } else {
//             //pai
//             dup2(fd[in],in);
//             close(fd[out]);
//             close(fd[in]);
//         }
//     }
//     pid = fork();
//     if (pid == 0) {
//         // flho
//         execvp(params[len-1][0],params[len-1]);
//     } else {
//         // pai
//         int res;
//         while (1) {
//         pid = wait(&res);
//         if (pid == -1)
//             break;
//         printf("Processo %d encerrou.\n", pid);
//         }
//     }
// }


    /* BEGIN -------------------------- building arg_list --------------------------
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


/*
// >
void func_out(char *params[], char *out){
    // char * in = "in.txt";
    // char * out = "out.txt";

    // int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY | O_CREAT, 0);

    //dup2(fdin, 0);
    dup2(fdout, 1);

    // close(fdin);
    close(fdout);

    // char * params[] = {"ls",NULL};
    execvp(params[0], params);

    return;
}

// <
void func_in(char *params[], char *in_file){
    // char * in = "in.txt";
    // char * out = "out.txt";

    int fdin = open(in_file, O_RDONLY, 0);
    //int fdout = open(out, O_WRONLY | O_CREAT, 0);

    dup2(fdin, 0);
    //dup2(fdout, 1);

    close(fdin);
    //close(fdout);

    // char * params[] = {"ls",NULL};
    execvp(params[0], params);

    return;
}

// 2>
void func_error(char *params[], char *out){
    // char * in = "in.txt";
    // char * out = "out.txt";
    // char * error = "error.txt";

    // int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY|O_APPEND, 0);
    // int fderror = open(error, O_WRONLY, 0);

    // dup2(fdin, 0);
    // dup2(fdout, 1);
    dup2(fdout, 2);

    // close(fdin);
    // close(fdout);
    close(fdout);

    // char * params[] = {"rm", "saidaaaaa.txt", NULL};
    execvp(params[0], params);

    return;
}
*/