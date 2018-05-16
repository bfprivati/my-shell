#define MAX_ARR_SIZE 1024
#define MAX_CMD_SIZE 1024
#define ANSI_ESCAPE_SEQUENCE(EscapeSeq)   "\33[" EscapeSeq
#define CTRLL 0xOC
#define in 0
#define out 1

void clear_input() {
    fflush(stdin);
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
        printf("-> ");
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

    i = 0;
    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        //printf("%s \n\n\n", params[i]);
        token = strtok(NULL, " ");

        if ( strcmp(params[i], "exit") == 0 ){
        // Sair do terminal OK

            return 0;
        } else if (strcmp(params[i], "cd") == 0){
        // Mover entre diretórios OK

            while(token != NULL) {
                params[i] = (char *) malloc(sizeof(strlen(token)));
                strcpy(params[i], token);
                token = strtok(NULL, " ");

                if( getcwd(cwdir, sizeof(cwdir)) != NULL) {
                    chdir(params[i]);
                }
                i++;
            } 

        } else if ( (strcmp(params[i], ">") == 0) || (strcmp(params[i], "<") == 0) || (strcmp(params[i], "2>") == 0)) {
            char *arquivo;
            i++;
            params[i] = (char *) malloc(sizeof(strlen(token)));
            printf("ARQUIVOOOOO %s", params[i]);

            strcpy(params[i], arquivo);
            token = strtok(NULL, " ");

            if (strcmp(params[i-1], ">") == 0) {
                // func_out(params, arquivo);
                printf("ENTROU FUNC_OUT");
            } else if (strcmp(params[i-1], "<") == 0) {
                printf("ENTROU FUNC_IN");
                // int fdin = open(params[i], O_RDONLY, 0);
                // dup2(fdin, 0);
                // close(fdin);
                // execvp(params[0], params);
            } else if (strcmp(params[i-1], "2>") == 0) {
                // func_error(params, arquivo);
                printf("ENTROU FUNC_ERROR");
            }

        } else if ((strcmp(params[i], "|") == 0)) {
            int fd[2];
            pipe(fd);
            int pid = fork();
            if (pid == 0) {
                close(fd[in]);
                dup2(fd[out], out);
                close(fd[out]);
                char* params[] = {"ls", "/etc/", NULL};
                execvp("ls", params);
            } else {
                close(fd[out]);
                dup2(fd[in], in);
                close(fd[in]);
                char* params[] = {"more", NULL};
                execvp("more", params);
            }
        }

        i++;
    }
    params[i] = token;
    execvp(params[0], params);

    return 1;
}

// // >
// void func_out(char *params[], char *out){
//     // char * in = "in.txt";
//     // char * out = "out.txt";

//     // int fdin = open(in, O_RDONLY, 0);
//     int fdout = open(out, O_WRONLY | O_CREAT, 0);

//     //dup2(fdin, 0);
//     dup2(fdout, 1);

//     // close(fdin);
//     close(fdout);

//     // char * params[] = {"ls",NULL};
//     execvp(params[0], params);

//     return;
// }

// // <
// void func_in(char params[], char in_file){
//     // char * in = "in.txt";
//     // char * out = "out.txt";

//     int fdin = open(in_file, O_RDONLY, 0);
//     //int fdout = open(out, O_WRONLY | O_CREAT, 0);

//     dup2(fdin, 0);
//     //dup2(fdout, 1);

//     close(fdin);
//     //close(fdout);

//     // char * params[] = {"ls",NULL};
//     execvp(params[0], params);

//     return;
// }

// // 2>
// void func_error(char *params[], char *out){
//     // char * in = "in.txt";
//     // char * out = "out.txt";
//     // char * error = "error.txt";

//     // int fdin = open(in, O_RDONLY, 0);
//     int fdout = open(out, O_WRONLY|O_APPEND, 0);
//     // int fderror = open(error, O_WRONLY, 0);

//     // dup2(fdin, 0);
//     // dup2(fdout, 1);
//     dup2(fdout, 2);

//     // close(fdin);
//     // close(fdout);
//     close(fdout);

//     // char * params[] = {"rm", "saidaaaaa.txt", NULL};
//     execvp(params[0], params);

//     return;
// }
