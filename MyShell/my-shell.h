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
