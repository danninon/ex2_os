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
int open_file(char* name);
void redirect(int fd_close, char* path_open);
void exit_error_msg();

void safe_pipe(int *pipefd);

void exec_stud_prog(const char *inp_path, char **new_argv);

void case_hello_who();

#define K_HELLO_WHO "hello_who"
#define K_HELLO "hello"
#define MAX_NAME_LEN 10
#define FILE_MAX_STUDENTS 100
#define MAX_FILE_NAME_LEN 1000
#define DYNAMIC_ARG_SIZE MAX_FILE_NAME_LEN*sizeof(char) + FILE_MAX_STUDENTS * MAX_NAME_LEN
//case1:
//argv[0] = ex2_q1
//argv[1] = symbol
//argv[2] = text_file path with names
//argv[3] = hello_who


typedef char* Name;
typedef char* Path;

int main(int argc, char* argv[]){





    char* inp_path = argv[1];

    int pid = fork();

    char** new_argv;//will be new argv for exec

    if (strcmp(inp_path, K_HELLO) == 0)
    { //argv == {"K_HELLO"}
        argv = &inp_path;
    } //only the name is needed for the at argv
    else if (strcmp(inp_path, K_HELLO_WHO) == 0) //only operate on these two
    {
        case_hello_who();

    }//need to read txt file to and each line insert to argv (use pipe)
    else
        exit_error_msg();
    //case redirect ( 3 parameters )
    if (argc != 4 && argc != 2)
        exit_error_msg();

    if (argc == 4) { //add redir
        if (pid == 0) {
            //case 3 parameters
            if (strcmp(inp_path, "1>") != 0 && strcmp(argv[1], "2>&1") && strcmp(argv[1], "1>") != 0)
                exit_error_msg();
            if (strcmp(inp_path, "1>") == 0 || strcmp(argv[1], "2>&1"))
                redirect(STDERR_FILENO, inp_path);
            if (strcmp(inp_path, "2>") == 0 || strcmp(argv[1], "2>&1"))
                redirect(STDOUT_FILENO, inp_path); //try with pipes
        }

    }
    execve(new_argv[0], new_argv, NULL);
    return 0;
}

void case_hello_who() {

    int num_of_sl
    //read stud_file - import from old project

    //allocate dynamic array

    //allocates static array
    char* new_argv[DYNAMIC_ARG_SIZE]; //argv[0] + argv[i] 0<i<student names in file
    new_argv[0] = K_HELLO;

    for
    //read students into array


    exec_stud_prog(inp_path, new_argv); //reads inp_path to new_argv


    //free dynamic array
}

void exec_stud_prog(const char *inp_path, char **new_argv) {
    char*  line = NULL;
    ssize_t nread;
    size_t len = 0;

    FILE* stream = fopen(inp_path, "r");
    if (stream == NULL) {
        exit_error_msg();
    }

    //read file lines to argv untill eof
    new_argv[0] = K_HELLO_WHO;
    nread = getline(&line, &len, stream) != -1;
    for (int i = 0 ; nread; ++i) {
       strcpy(new_argv[i],line);
        nread = getline(&line, &len, stream) != -1;
    }
    free(line);
    //i holds size +1
}

void safe_pipe(int *pipefd) {
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void redirect(int fd_close, char* path_open){
    close(fd_close);
    open_file(path_open);
}
void todo3() {

}

void todo2() {

}

void todo1() {

}
void exit_error_msg(){
    perror("Exec failed: ");
    fprintf(stderr, "Exec Failed, going to exit.\n");}
}

int open_file(char* name)
{
    int fd;
    fd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0)  // open failed
    {
        fprintf(stderr, "ERROR: open \"%s\" failed (%d). Exiting\n", name, fd);
        exit(2);
    }
    fprintf(stderr, "opened file %s, file descriptor is: %d\n",name, fd);
    return(fd);
}