#define MAX_ARR_SIZE 1024
#define MAX_CMD_SIZE 1024

// function to show prompt
int show_prompt(){
    char * user;
    char host[MAX_ARR_SIZE];
    char * home;
    char cwdir[MAX_ARR_SIZE];

    user = getenv("USER");
    gethostname(host, MAX_ARR_SIZE);
    home = getenv("HOME");

    if( getcwd(cwdir, sizeof(cwdir)) != NULL)
        printf("%s/%s%s -> ", user, host, cwdir);

    // tratar home/aluno de cwdir e ~ em home
}

// read command given by the user
int read_command(){
    char cwdir[MAX_ARR_SIZE];
    int i;
    char command[MAX_CMD_SIZE];
    char * token;
    char * params[MAX_ARR_SIZE];

    while (1) {
        fflush(stdin);
        signal(SIGHUP, SIG_IGN);    //bloquear KILL
        signal(SIGINT, SIG_IGN);    //bloquear CTRLC
        signal(SIGTSTP, SIG_IGN);
        scanf(" %[^\n]s", command);
        token = strtok(command, " ");

        i = 0;
        while(token != NULL) {
            params[i] = (char *) malloc(sizeof(strlen(token)));
            strcpy(params[i], token);
            token = strtok(NULL, " ");

            if (strcmp(params[i], "exit") == 0){
                exit(0);
            } else if (strcmp(params[i], "cd") == 0){
                    i++;
                    params[i] = (char *) malloc(sizeof(strlen(token)));
                    strcpy(params[i], token);
                    token = strtok(NULL, " ");
                    if(token == NULL){
                        char * home;
                        home = getenv("HOME");
                        chdir(home);
                        if( getcwd(cwdir, sizeof(cwdir)) != NULL)
                        printf("%s -> \n\n\n", cwdir);
                    } else if( getcwd(cwdir, sizeof(cwdir)) != NULL) {
                        chdir(params[i]);
                        if( getcwd(cwdir, sizeof(cwdir)) != NULL)
                        printf("%s -> \n\n\n", cwdir);
                    }
            }

            i++;
        }
    };
    return 1;
}

// >
int func_out(){
    //char * in = "in.txt";
    char * out = "out.txt";

    //int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY, 0);

    //dup2(fdin, 0);
    dup2(fdout, 1);

    //close(fdin);
    close(fdout);

    char * params[] = {"ls",NULL};
    execvp("ls",params);

    return 0;
}

// >>
int func_append(){
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

    return 0;
}

// 2>
int func_error(){
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

    return 0;
}

// 2>>
int func_error_append(){
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

    return 0;
}
