#define MAX_ARR_SIZE 1024
#define MAX_CMD_SIZE 1024
#define MAX_LIST_SIZE 1024
#define MAX_PIPE_SIZ 24         //variável string do PIPE
#define ANSI_ESCAPE_SEQUENCE(EscapeSeq)   "\33[" EscapeSeq
#define in 0
#define out 1

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define BOLD "\x1b[1m"

int redir_out = 0; /* Se (>), a variável redir_out será setada para 1    */
int redir_in = 0;  /* Se (<), a variável redir_in será setada para 1     */
int redir_err = 0; /* Se (2>), a variável redir_err será setada para 1   */
int cmd = 0;
int size_command_list;
char* command_list[MAX_ARR_SIZE];


// Mostrar terminal
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

    printf("%s%s%s@%s%s:%s%s%s%s", BOLD, ANSI_COLOR_GREEN, user, host, BOLD, ANSI_COLOR_RED, ANSI_COLOR_BLUE, params[0], ANSI_COLOR_RESET);
    i++;

    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        token = strtok(NULL, "/");
        if ( strcmp(params[i], user) == 0 )
            params[i] = "";

        printf("%s%s%s/%s", BOLD, ANSI_COLOR_BLUE, params[i], ANSI_COLOR_RESET);
        i++;
    }
        printf(" $ ");
    return;
}

void tratar_signal (int signum){
	printf("\n");
}

//Trata CTRLC, CTRLV E KILL
void signal_handler (){
    signal(SIGHUP, SIG_IGN);    // bloquear KILL
    signal(SIGINT, SIG_IGN);    // bloquear CTRLC
    signal(SIGTSTP, SIG_IGN);   // bloquear CTRLZ

	struct sigaction new_action; //struct da System Call Sigaction
	memset(&new_action, 0, sizeof(new_action)); //inicializar a variavel
	new_action.sa_handler = &tratar_signal; //recebe nova acao

	sigaction(SIGINT, &new_action, NULL); 	//CTRL+C
	sigaction(SIGTSTP, &new_action, NULL); 	//CTRL+Z
}

//Trata comandos com pipe " | "
int spawn_process(char *params[MAX_PIPE_SIZ][MAX_PIPE_SIZ], int len) {
    // SOMENTE USAR QUANDO LS -LA | SORT | MORE
    // USAR ARRAY DE ARRAYS, TRATANDO |
    printf("\nEntrou no spawn_process\n");

    int i, status;
    int fd[2];
    int pid;
    int pipe_count;
    pipe_count = 0;

    for(i=0; i < len -1 ; i++) {
        //printf("Entrou no for\n");
        pipe(fd);
        pid = fork();
        if (pid == 0) {
            //printf("Entrou no for - filho1\n");
            // filho
            dup2(fd[out],out);
            close(fd[out]);
            close(fd[in]);
            execvp(params[i][0],params[i]);
        } else {
            //printf("Entrou no for - pai\n");
            //pai
            dup2(fd[in],in);
            close(fd[out]);
            close(fd[in]);
        }
    }

    pid = fork();
    if (pid == 0) {
        // flho
    //    printf("Entrou no filho\n");
        execvp(params[len-1][0],params[len-1]);
    } else {
      //  printf("Entrou no pai\n");
        // pai
        //int res;
        /*while (pipe_count < len) {
            printf("Entrou no while?\n");
            pid = wait(&res);

            //if (pid == -1)
            //if (WIFEXITED(res) && (WEXITSTATUS(res)==0))
                //break;
            printf("Processo %d encerrou.\n", pid);
            pipe_count ++;
        }*/
        dup2(in, STDIN_FILENO);
    }
    for (i = 0; i < len+1; i++)
        wait(&status);

    printf("retornar\n");
}

//cria processos para diversos comandos
int create_process(char **params, char *arquivo){
    int fd, error;
    int pid = fork();

    if (pid == 0) {
    // codigo do processo filho
        if(redir_in == 1 && arquivo != NULL){
            fd = open(arquivo, O_RDONLY, 0777);
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
        error = execvp(params[0], params);
        close(fd);

        if(error != NULL){
            printf("ENTROU IF ERROR \n");
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

//Limpa as strings que armazenam comandos e parâmetros 
void zera_string(char *command, char ** params){
    int j;

    for (j=0; j< MAX_CMD_SIZE; j++){
      command[j] = '\0';
    }
    for (j=0; j< MAX_ARR_SIZE; j++){
      params[j] = '\0';
    }
    //token = '\0';
}

// void zera_string2(char *command, char **params){
//     int j, i;

//     for (j=0; j< MAX_CMD_SIZE; j++){
//         command[j] = '\0';
//     }
//     for(i=0; i< MAX_PIPE_SIZ; i++){
//         for (j=0; j< MAX_PIPE_SIZ; j++){
//             *params[i][j] = '\0';
//         }
//     }
// }

//Lê entrada de comando e faz o parser, chama funções de tratamento dos comandos
int read_command() {
    int i=0, j=0, params_count, dir;
    int parm = 0;                          //Variáveis Pipe
    char cwdir[MAX_ARR_SIZE];
    char command[MAX_CMD_SIZE];
    char * token;
    char * params[MAX_ARR_SIZE];
    char * arquivo = NULL;
    char commandpipe[MAX_CMD_SIZE];                 //Variável String completa de comando do Pipe
    char * paramspipe[MAX_PIPE_SIZ][MAX_PIPE_SIZ];  //Variável String Pipe Parser

    //Zerar string
    zera_string (command, params);
    //strcpy(commandpipe, command);       //Salva comando no commandpipe
    // zera_string2 (commandpipe, paramspipe);

    // Tratar sinal
    //signal_handler();

    // ler comando e tirar espaços
    cmd = 0 ;
    token = 0;
    printf("EU AQUI ASASHWUSH\n");

    fflush(stdin);
    fgets(command, sizeof(char) * MAX_ARR_SIZE, stdin);
    if(command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';

    fflush(stdin);
    strcpy(commandpipe, command);       //Salva comando no commandpipe
    token = strtok(command, " ");

    printf("COMANDO SIM: %d\n", cmd);
    printf("TOKEN: %s\n", token);

    while(token != NULL) {
        params[i] = (char *) malloc(sizeof(strlen(token)));
        strcpy(params[i], token);
        token = strtok(NULL, " ");

        printf("\nENTROU WHILE PRINCIPAL\n");
        printf("PARAMETR DENTRO DO WHILE %s\n", params[i]);


        if ( strcmp(params[i], "exit") == 0 ){
        // Sair do terminal OK

            exit(0);
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

            zera_string (command, params);
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
        else if ( strcmp (params[i], "|") == 0){
        //Tratamento de Pipe
            zera_string (command, params);

            //Parser dos Comandos do Pipe
            token = strtok(commandpipe, " ");
            while (token != NULL ){

                printf("\nENTROU WHILE DO PIPE\n");
                printf("PARAMETR DENTRO DO WHILEP %s\n", params[i]);

                if ( strcmp (token, "|") == 0 ){
                    paramspipe[cmd][parm] = NULL;

                    cmd ++;
                    parm = 0;

                    token = strtok(NULL, " ");
                    paramspipe[cmd][parm] = (char*) malloc (sizeof (strlen(token)));
                    strcpy(paramspipe[cmd][parm], token);
                    //printf("Parametro[%d][%d]: %s\n", cmd, parm, paramspipe[cmd][parm] );
                    parm++;

                } else {
                    paramspipe[cmd][parm]= (char*) malloc (sizeof (strlen(token)));
                    strcpy(paramspipe[cmd][parm], token);
                    //printf("Parametro[%d][%d]: %s\n", cmd, parm, paramspipe[cmd][parm] );
                    parm ++;
                }
                token = strtok(NULL, " ");

            }
            paramspipe[cmd][parm] = NULL;
            //Função que cria processos para tratar os pipes
            spawn_process (paramspipe, cmd + 1);  printf("VOLTOU DO SPWAN_PROCESS\n");

        }
        i++;
    }
    // params_count++;
    // insert_command_list(params, params_count);

    printf ("****VEIO AQUI****\n\n");
    //if(cmd == 0 ){
        create_process (params, arquivo); printf("VOLTOU DO CREATE_PROCESS\n");
        //zera_string2 (commandpipe, paramspipe);

    return 1;
}




/* --------------------------------------------------------------------------------------------------------------------------*/





