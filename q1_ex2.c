//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

//consts
#define K_HELLO_WHO "hello_who"
#define K_HELLO "hello"
#define MAX_ARGS 100
#define MAX_ARG_LEN 1000

//headers
void Q2(char *argv[]);
void open_file(char *name);
void redirect(int fd_close, char *path_open);
void exit_error_msg();
void case_hello_who(char **new_argv, char *org_argv[], int effective_index);
void clean_standard_buffer();
int safe_fork();
int case_pipe(char *const *argv, int program_index);
void run_program(char *argv[], int program_index);

int main(int argc, char *argv[]) {
    Q2(argv);
    return 0; //gets here only on success
}

void Q2(char *argv[]) {
    int program_index = 1; //default no pipe
    clean_standard_buffer();

    int pid = safe_fork();
    if (pid == 0) { //son
        if (strcmp(argv[1], "1>") == 0 || strcmp(argv[1], "2>") == 0 || strcmp(argv[1], "2>&1") == 0)        //makes the next if with strcmp instead with || between
            program_index =  case_pipe(argv, program_index); //3 + do redirections on both ends

        if ((strcmp(argv[program_index], K_HELLO_WHO) != 0) && strcmp(argv[program_index], K_HELLO) != 0){
            FILE* fp = fopen(argv[program_index], "r"); //should return throw error if not exist a
            if (fp == NULL) //should be true always
                exit_error_msg();
        }
        run_program(argv, program_index);
    } else { //father
        wait(NULL);
    }
}

void run_program(char *argv[], int program_index) {
    //init new argv
    char **new_argv = malloc(sizeof(char*)*(MAX_ARGS+1));
    for (int i = 0; i < MAX_ARGS+1; ++i) {
        new_argv[i] = malloc(sizeof(char)*MAX_ARG_LEN);
  }
    strcpy(new_argv[0], argv[program_index]);
    if (strcmp(argv[program_index], K_HELLO) == 0) {
        new_argv[1] = NULL;
    } else { // case hello
        case_hello_who(new_argv, argv, program_index);
    }
    execve(new_argv[0], new_argv, NULL);
}

int case_pipe(char *const *argv, int program_index) {
    program_index = 3;

    int out_file_path_index = program_index - 1;
    if (strcmp(argv[program_index], "2>&1") == 0) {//can make const
        redirect(STDERR_FILENO, argv[out_file_path_index]);
        redirect(STDOUT_FILENO, argv[out_file_path_index]);
    }
    else if (strcmp(argv[1], "1>") == 0) {
        redirect(STDOUT_FILENO, argv[out_file_path_index]);
    }
    else if (strcmp(argv[1], "2>") == 0) {
        redirect(STDERR_FILENO, argv[out_file_path_index]);
    }
    return program_index;
}

void exit_error_msg() {
    perror("Exec failed: ");
    fprintf(stderr, "Exec Failed, going to exit.\n");
    exit(errno);
}

void case_hello_who(char **new_argv, char *org_argv[], int effective_index) {
    //allocates
    int i;
    for ( i = 1; org_argv[effective_index + i]; i++) {
        new_argv[i] = org_argv[effective_index + i];
    }
    new_argv[i] = NULL;
}

void redirect(int fd_close, char *path_open) {
    close(fd_close);
    open_file(path_open);
}

void open_file(char *name) {
    FILE* fp = fopen(name , "w");
    if (fp == NULL)  // open failed
    {
        fprintf(stderr, "ERROR: open \"%s\" failed Exiting\n", name);
        exit(2);
    }
    fprintf(stderr, "opened file %s\n", name);
}

int safe_fork() {
    int pid = fork();
    if (pid == -1)
        exit_error_msg();
    return pid;
}

void clean_standard_buffer() {
    setbuf(stderr, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);
}

