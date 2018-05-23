#define MAX_ARR_SIZE 1024
#define MAX_CMD_SIZE 1024
#define ANSI_ESCAPE_SEQUENCE(EscapeSeq)   "\33[" EscapeSeq
#define CTRLL 0xOC
#define entrada STDIN_FILENO
#define saida STDOUT_FILENO

void clear_input() {
    fflush(stdin);
    return;
}

void io_rdrct(in, out){
    if (in != STDIN_FILENO) {
      dup2(in, STDIN_FILENO);
      close(in);
    }

    if (out != STDOUT_FILENO) {
      dup2(out, STDOUT_FILENO);
      close(out);
    }

    return;
}

// function to show prompt
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

// read command given by the user
int read_command(){
    int i;
    char cwdir[MAX_ARR_SIZE];
    char command[MAX_CMD_SIZE];
    char * token;
    char * params[MAX_ARR_SIZE];

	signal_handler();

    // ler comando e tirar espaços
    scanf(" %[^\n]s", command);
    token = strtok(command, " ");

    if(token == NULL) {
        return 1;
    }

    i = 0;
    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        token = strtok(NULL, " ");

        if ( strcmp(params[i], "exit") == 0 ){
        // Sair do terminal OK

            return -2;
        } else if (strcmp(params[i], "cd") == 0){
        // Mover entre diretórios OK

            // if( (token == NULL) || (strcmp(params[i], ">")) ){
            //         char homeDir[50];
            //         strcpy(homeDir, getenv("HOME"));
            //         params[i] = (char *) malloc(sizeof(strlen(token)));
            //         strcpy(params[i], homeDir);
            //         chdir(params[i]);
            // }
            while(token != NULL) {
                int dir = 0;
                params[i] = (char *) malloc(sizeof(strlen(token)));
                strcpy(params[i], token);
                token = strtok(NULL, " ");

                if( getcwd(cwdir, sizeof(cwdir)) != NULL) {
                    dir = chdir(params[i]);
                    if(dir != 0)
                        fprintf(stderr, "cd: '%s' file or directory not found\n", params[i]);
                }
                i++;
            }
        }

        i++;
    }
    params[i] = token;
    create_process(params);
    //return execvp(params[0], params);

    return 1;
}

int create_process(char **params){
    execvp(params[0], params);

//     if ( (strcmp(params[i], ">") == 0) || (strcmp(params[i], "<") == 0) || (strcmp(params[i], "2>") == 0)) {
//         char *arquivo;
//         i++;
//         params[i] = (char *) malloc(sizeof(strlen(token)));
//         strcpy(params[i], arquivo);
//         token = strtok(NULL, " ");
//         printf("ARQUIVOOOOO %s", params[i]);

//         if (strcmp(params[i-1], ">") == 0) {
//             // char * out = params[i];
//             // int fdout = open(out, O_WRONLY | O_CREAT, 0);
//             // dup2(fdout, 1);

//             printf("ENTROU FUNC_OUT");
//         } else if (strcmp(params[i], "<") == 0) {
//             //func_in(params, arquivo);
//             printf("ENTROU FUNC_IN");
//         } else if (strcmp(params[i], "2>") == 0) {
//             // func_error(params, arquivo);
//             printf("ENTROU FUNC_ERROR");
//         }

//     } else if ((strcmp(params[i], "|") == 0)) {
//         int fd[2];
//         pipe(fd);
//         int pid = fork();
//         if (pid == 0) {
//             close(fd[in]);
//             dup2(fd[out], out);
//             close(fd[out]);
//             execvp(params[0], params);
//         } else {
//             close(fd[out]);
//             dup2(fd[in], in);
//             close(fd[in]);
//             execvp(params[0], params);
//         }
//     }


    return 1;
}

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