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

    // if(cwdir != NULL)
    //     printf("%s@%s %s ->", user, host, cwdir);

    // if(cwdir != NULL)
    //     printf("%s@%s ", user, host);

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
        printf("%s \n\n\n", params[i]);
        token = strtok(NULL, " ");
        
        if ( strcmp(params[i], "exit") == 0 ){
            return 0;
        } else if (strcmp(params[i], "cd") == 0){
        // Mover entre diretórios
            
            char * home;
            strcpy(home, getenv("HOME"));
            
            while(token != NULL) {
                params[i] = (char *) malloc(sizeof(strlen(token)));
                strcpy(params[i], token);
                token = strtok(NULL, " ");
            
                if( getcwd(cwdir, sizeof(cwdir)) != NULL) {
                    chdir(params[i]);
                } 
                // else if( strcmp(params[i], "~")==0){
                //     chdir(home);
                //     // if( getcwd(cwdir, sizeof(cwdir)) != NULL)
                //     // printf("%s -> \n\n\n", cwdir);
                // }
                i++;
            }
        } else if ( (strcmp(params[i], ">") == 0) || (strcmp(params[i], "<") == 0) || (strcmp(params[i], "2>") == 0)) {
            char arquivo;
            i++;
            params[i] = (char *) malloc(sizeof(strlen(token)));
            printf("ARQUIVOOOOO %s", params[i]);

            //strcpy(params[i], arquivo);
            //token = strtok(NULL, " ");
            /*
            switch (params[i-1]) {
                case ">":
                    func_out(params, arquivo);
                break;
            
                case "<":
                    func_append(params, arquivo);
                break;

                case "2>":
                    func_error(params, arquivo);
                break;

                default:
                break;
            } */

        } /*else if ((strcmp(params[i], "|") == 0)) {

        }*/

        i++;
    }   
    
    execvp(params[0], params);
    return 1;
}

/*
// >
void func_out(char *params[], char *out){
    // char * in = "in.txt";
    // char * out = "out.txt";

    // int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY, 0);

    //dup2(fdin, 0);
    dup2(fdout, 1);

    // close(fdin);
    close(fdout);

    // char * params[] = {"ls",NULL};
    execvp(params[0], params);

    return;
}

// <
void func_out(char *params[], char *out){
    // char * in = "in.txt";
    // char * out = "out.txt";

    // int fdin = open(in, O_RDONLY, 0);
    int fdout = open(out, O_WRONLY, 0);

    //dup2(fdin, 0);
    dup2(fdout, 1);

    // close(fdin);
    close(fdout);

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