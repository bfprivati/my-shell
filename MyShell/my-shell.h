#define MAX_ARR_SIZE 1024
#define MAX_CMD_SIZE 1024
#define ANSI_ESCAPE_SEQUENCE(EscapeSeq)   "\33[" EscapeSeq
#define CTRLL 0xOC

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
        if ( strcmp(params[i], "aluno") == 0 )
            params[i] = "";
        
        printf("%s/", params[i]);
        i++;
    }
        printf("-> ");
    return;
}

// read command given by the user
int read_command(){
    int i;
    char cwdir[MAX_ARR_SIZE];
    char command[MAX_CMD_SIZE];
    char * token;
    char * params[MAX_ARR_SIZE];

    signal(SIGHUP, SIG_IGN);    // bloquear KILL
    signal(SIGINT, SIG_IGN);    // bloquear CTRLC
    signal(SIGTSTP, SIG_IGN);   // bloquear CTRLZ

    // ler comando e tirar espaços
    scanf(" %[^\n]s", command);
    token = strtok(command, " ");

    i = 0;
    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        token = strtok(NULL, " ");

        if(params[i] == '\0'){
        // Se comando vazio, mostra terminal novamente

            printf("\n");
        } else if (strcmp(params[i], "exit") == 0){
        // Sai do programa

            exit(0);
        } else if (strcmp(params[i], "cd") == 0){
        // Mover entre diretórios
            
            char * home;
            home = getenv("HOME");
            
            i++;
            while(token != NULL) {
                params[i] = (char *) malloc(sizeof(strlen(token)));
                strcpy(params[i], token);
                token = strtok(NULL, " ");
                // token = strtok(NULL, "");
            
                if( getcwd(cwdir, sizeof(cwdir)) != NULL) {
                    chdir(params[i]);
                    // if( getcwd(cwdir, sizeof(cwdir)) != NULL)
                    // printf("DIRETORIO %s -> \n\n\n", cwdir);
                } 
                // else if( strcmp(params[i], "~")==0){
                //     chdir(home);
                //     // if( getcwd(cwdir, sizeof(cwdir)) != NULL)
                //     // printf("%s -> \n\n\n", cwdir);
                // }
                i++;
            } 
            // if (token == NULL) {
            //     chdir(home);
            //     return 1;
            // }
        } else if (strcmp(params[i], "clear") == 0) {
        // Limpa terminal

            write(1, "\33[H\33[2J", 7);
        }
        // } else if (strcmp(params[i], "ls") == 0) {
        // // Mostra diretório

        //     struct dirent **namelist;
        //     int n;
        //     if(argc < 1) {
        //         exit(EXIT_FAILURE);
        //     } else if (argc == 1) {
        //         n=scandir(“.”,&namelist,NULL,alphasort);
        //     } else {
        //         n = scandir(argv[1], &namelist, NULL, alphasort);
        //     }
        //     if(n < 0) {
        //         perror(“scandir”);
        //         exit(EXIT_FAILURE);
        //     } else {
        //         while (n–) {
        //             printf(“%s\n”,namelist[n]->d_name);
        //             free(namelist[n]);
        //         }
        //         free(namelist);
        //     }
        //     exit(EXIT_SUCCESS);
        // }
        // i++;

    //     if ( (strcmp(params[i], ">") == 0) || (strcmp(params[i], ">>") == 0) || (strcmp(params[i], "2>") == 0)) {
    //     // Redirecionamento de arquivos
            
    //         i++;
    //         while(token != NULL) {
    //             params[i] = (char *) malloc(sizeof(strlen(token)));
    //             strcpy(params[i], token);
    //             token = strtok(NULL, " ");

    //             if( getcwd(cwdir, sizeof(cwdir)) != NULL) {
    //                 chdir(params[i]);
    //                 // if( getcwd(cwdir, sizeof(cwdir)) != NULL)
    //                 // printf("DIRETORIO %s -> \n\n\n", cwdir);
    //             } else if(params[i] == '\0'){
    //                 char * home;
    //                 home = getenv("HOME");
    //                 printf("HOMI: %s", home);
    //                 chdir(home);
    //                 // if( getcwd(cwdir, sizeof(cwdir)) != NULL)
    //                 // printf("%s -> \n\n\n", cwdir);
    //             }
    //             i++;
    //         }
            
    //         i++;
    //         params[i] = (char *) malloc(sizeof(strlen(token)));
    //         strcpy(params[i], token);
    //         token = strtok(NULL, " ");

    //         char * ant, prox;
    //         strcpy(ant, params[i-1]);
    //         i++;
    //         params[i] = (char *) malloc(sizeof(strlen(token)));
    //         strcpy(params[i], token);

    //         //func_out(ant, prox);
    //     }
    }
    return 1;
}

// >
void func_out(){
    char * in = "in.txt";
    char * out = "out.txt";

    //int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY, 0);

    //dup2(fdin, 0);
    dup2(fdout, 1);

    //close(fdin);
    close(fdout);

    char * params[] = {"ls",NULL};
    execvp("ls",params);

    return;
}

// >>
void func_append(){
    char * in = "in.txt";
    char * out = "out.txt";

    int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY|O_APPEND, 0);

    dup2(fdin, 0);
    dup2(fdout, 1);

    close(fdin);
    close(fdout);

    char * params[] = {"sort",NULL};
    execvp("sort",params);

    return;
}

// 2>
void func_error(){
    char * in = "in.txt";
    char * out = "out.txt";
    char * error = "error.txt";

    int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY|O_APPEND, 0);
    int fderror = open(error, O_WRONLY, 0);

    dup2(fdin, 0);
    dup2(fdout, 1);
    dup2(fderror, 2);

    close(fdin);
    close(fdout);
    close(fderror);

    char * params[] = {"rm", "saidaaaaa.txt", NULL};
    execvp("rm",params);

    return;
}

// 2>>
void func_error_append(){
    char * in = "in.txt";
    char * out = "out.txt";
    char * error = "error.txt";

    int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY|O_APPEND, 0);
    int fderror = open(error, O_WRONLY|O_APPEND, 0);

    dup2(fdin, 0);
    dup2(fdout, 1);
    dup2(fderror, 2);

    close(fdin);
    close(fdout);
    close(fderror);

    char * params[] = {"rm", "saidaaaaa.txt", NULL};
    execvp("rm",params);

    return;
}
