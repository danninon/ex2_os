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

int open_file(char *name);

void redirect(int fd_close, char *path_open);

void exit_error_msg();

void safe_pipe(int *pipefd);

void exec_stud_prog(const char *inp_path, char **new_argv);

void case_hello_who();

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



void case_hello(char **new_argv, char *inp_path);

void case_hello_who(char **new_argv, char *names_file);

void clean_standard_buffer();

int main(int argc, char *argv[]) {
    clean_standard_buffer();

    int pid = fork();
    if (pid == -1) {
        exit_error_msg();
    }

    char **new_argv = NULL;

    char *inp_path = argv[1];

    if (pid == 0) { //son goes brrrr
        if (strcmp(inp_path, K_HELLO) == 0) { //argv == {"K_HELLO"}
            case_hello(new_argv, inp_path);
        } //only the name is needed for the at argv
        else if (strcmp(inp_path, K_HELLO_WHO) == 0) //only operate on these two
        {
            case_hello_who(new_argv, inp_path);
        }//need to read txt file to and each line insert to argv (use pipe)
        else //not valid program(assignment requested)
            exit_error_msg();

        //redirect if needed
        //case invalid input
        if (argc != 4 && argc != 2)
            exit_error_msg();
            //case redirect ( 3 parameters )
        else if (argc == 4) { //add redir
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
        //runs program
        execve(new_argv[0], new_argv, NULL); //will be freed since no one is pointering it
    } else {
        wait(NULL); //dad goes zZzZz


    }
    return 0;
}

void clean_standard_buffer() {
    setbuf(stderr, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);
}


void case_hello(char **argv, char *prog_name) {
    argv = calloc(2, sizeof(MAX_ARG_LEN)); //could be made smaller, not bothering.
    strcpy(argv[0],K_HELLO_WHO);
}

FILE *open_read(char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        exit_error_msg();
    }
    return fp;
}

void case_hello_who(char **new_argv, char *file_path) {
    //allocates
    new_argv = malloc(sizeof(char));
    for (int i = 0; i < FILE_MAX_STUDENTS; i++) {
        new_argv[i] = malloc(MAX_NAME_LEN);
    }
    strcpy(new_argv[0],K_HELLO_WHO);
    char line_buf[MAX_NAME_LEN + 1];

    /* Open the file for reading */
    FILE *in_fp = open_read(file_path);
    int i = 1;
    //clean buffer pls
    while (fgets(line_buf, MAX_NAME_LEN, in_fp) != NULL) {
        strcpy(new_argv[i++], line_buf);
        // ((ch = getc(in_fp)) != EOF) && (ch != '\n')
        // fseek(in_fp, -1, SEEK_CUR); //returns effect of getc (removes from line_buf)
        //get next student name
    }
    fclose(in_fp);
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