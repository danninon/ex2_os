#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/types.h>
#include <unistd.h>     // for sleep(), execvp(), pipe()
#include <sys/wait.h>   // for wait()
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <bits/fcntl-linux.h>
#include <sys/stat.h>
#include <regex.h>
#include <stdbool.h>

int open_file(char *name);

void redirect(int fd_close, char *path_open);

void exit_error_msg();

void safe_pipe(int *pipefd);

void exec_stud_prog(const char *inp_path, char **new_argv);


#define TXT_REG = "/.*\.txt"
#define K_HELLO_WHO "hello_who"
#define K_HELLO "hello"
#define MAX_NAME_LEN 10
#define FILE_MAX_STUDENTS 100
#define MAX_ARGS 100
#define MAX_ARG_LEN 1000
#define MAX_FILE_NAME_LEN 1000
#define DYNAMIC_ARG_SIZE MAX_FILE_NAME_LEN*sizeof(char) + FILE_MAX_STUDENTS * MAX_NAME_LEN
//case1:
//argv[0] = ex2_q1
//argv[1] = symbol
//argv[2] = text_file path with names
//argv[3] = hello_who


void case_hello_who(char **new_argv, char *org_argv[], int effective_index);

void clean_standard_buffer();

void init_new_argv(char *prog_path_to_exec, char ***new_argv);

__attribute__((unused)) void deallocate_son(char **new_argv);

__attribute__((unused)) bool check_text_with_regex(char *word);

int safe_fork();

int case_pipe(char *const *argv, int program_index);

void case_legal_program(char *argv[], int program_index);

void Q2(char *argv[]);


void test(char **const *new_argv, char *path);

int main(int argc, char *argv[]) {
    Q2(argv);
    return 0;

}

void Q2(char *argv[]) {
    int program_index = 1; //default no pipe
    clean_standard_buffer();


    int pid = safe_fork();
    if (pid == 0) {
        fprintf(stderr, "child's pid: %d parent pid: %d\n", getpid(), getppid());
        //makes the next if with strcmp instead with || between
        if (strcmp(argv[1], "1>") == 0 || strcmp(argv[1], "2>") == 0 || strcmp(argv[1], "2>&1") == 0) {
            program_index =  case_pipe(argv, program_index); //3 + do redirections on both ends
        }
        if ((strcmp(argv[program_index], K_HELLO_WHO) != 0) && strcmp(argv[program_index], K_HELLO) != 0)
            exit_error_msg();
        //legal
        //just used for convenience since many pointers are used, gets set at init
        case_legal_program(argv, program_index);

    } else {
        fprintf(stderr, "father's pid: %d\n", getpid());
        wait(NULL);
    }
}

void case_legal_program(char *argv[], int program_index) {
    //init new argv
    char **new_argv = malloc(sizeof(char*)*(MAX_ARGS+1));
     for (int i = 0; i < MAX_ARGS+1; ++i) {
        new_argv[i] = malloc(sizeof(char)*MAX_ARG_LEN);
  }
  //set argv[0]
    strcpy(new_argv[0], argv[program_index]);
    if (strcmp(argv[program_index], K_HELLO)) {
        new_argv[1] = NULL;

    } else { // case hello
        case_hello_who(new_argv, argv, program_index);
    }
    fprintf(stderr, "new_argv[0]: %s\n", new_argv[0]);
    execve(new_argv[0], new_argv, NULL);
}

int case_pipe(char *const *argv, int program_index) {
    fprintf(stderr, "case pipe\n");
    program_index = 3;
    if (strcmp(argv[program_index], "1>") != 0) //can make const
        redirect(STDOUT_FILENO, argv[program_index]);
    else if (strcmp(argv[1], "2>"))
        redirect(STDERR_FILENO, argv[program_index]);
    else {
        redirect(STDOUT_FILENO, argv[program_index]);
        redirect(STDERR_FILENO, argv[program_index]);
    }
    return program_index;
}

int safe_fork() {
    int pid = fork();
    if (pid == -1) {
        exit_error_msg();
    }
    return pid;
}
//
//void init_new_argv(char *prog_path_to_exec, char ***new_argv) {
//    *new_argv = malloc(MAX_ARGS * sizeof(char *));
//    if (*new_argv == NULL) {
//        exit_error_msg();
//    }
//    (*new_argv)[0] = prog_path_to_exec;
//    for (int i = 1; i < MAX_ARGS; i++) {
//        (*new_argv)[i] = NULL;
//    }
//
//    fprintf(stderr, "going to load: %s to new_argv[0]\n", prog_path_to_exec);
//    fprintf(stderr, "size of allocated argv: %ld\n\n\n", (**new_argv));
//    sleep(1);
//    fprintf(stderr,"\n\nsize of allocated: %ld\n", sizeof(*new_argv));
//    sleep(1);
//    char res = strcpy(**new_argv, prog_path_to_exec);
//   fprintf("%c", &res);
//    fprintf(stderr, "new_argv[0]: %s\n", (*new_argv)[0]);
//}


void test(char **const *new_argv, char *path) {

    fprintf(stderr, "Loading...\n");
    fprintf(stderr, "beep\nnew_argv[0]: %s\n", (*new_argv)[0]);
    sleep(1);
    fprintf(stderr, "finished init\nboop\n");
}

void clean_standard_buffer() {
    setbuf(stderr, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);
}

void case_hello_who(char **new_argv, char *org_argv[], int effective_index) {
    //allocates
    int i;
    fprintf(stderr, "printing new argv:\n");
    for ( i = 1; org_argv[effective_index + i]; i++) {
        new_argv[i] = org_argv[effective_index + i];
        fprintf(stderr, "new_argvg[%d]: %s\n", i, new_argv[i]);
    }
    new_argv[i] = NULL;
    fprintf(stderr, "new_argv[%d]: %s\n", i, new_argv[i]);
}

void safe_pipe(int *pipefd) {
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void redirect(int fd_close, char *path_open) {
    close(fd_close);
    open_file(path_open);
}

void exit_error_msg() {
    perror("Exec failed: ");
    fprintf(stderr, "Exec Failed, going to exit.\n");
}


int open_file(char *name) {
    int fd;
    fd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0)  // open failed
    {
        fprintf(stderr, "ERROR: open \"%s\" failed (%d). Exiting\n", name, fd);
        exit(2);
    }
    fprintf(stderr, "opened file %s, file descriptor is: %d\n", name, fd);
    return (fd);
}

