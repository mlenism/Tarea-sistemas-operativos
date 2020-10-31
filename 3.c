#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define READ  0
#define WRITE 1

void error(int *fd) {
    if (pipe(fd) == -1) {
        perror("Creating pipe");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int   fd[2];

    if (argc == 2) {
        execlp(argv[1], argv[1], NULL);
    } else if (argc == 3) {
        execlp(argv[1], argv[1], argv[2], NULL);
    } else if (argc > 4) {
        if (argv[2] == "|") {
            if (argc == 4) {
                error(fd);
                switch(pid = fork()) {
                    case 0:
                        close(fd[WRITE]);
                        dup2(fd[READ], STDIN_FILENO);
                        execlp(argv[3], argv[3], NULL);

                    case -1:
                        perror("fork() failed)");
                        exit(EXIT_FAILURE);

                    default:
                        close(fd[READ]);
                        dup2(fd[WRITE], STDOUT_FILENO);
                        execlp(argv[1], argv[1], NULL);
                }
            } else {
                error(fd);
                switch(pid = fork()) {
                    case 0:
                        close(fd[WRITE]);
                        dup2(fd[READ], STDIN_FILENO);
                        execlp(argv[3], argv[3], argv[4], NULL);

                    case -1:
                        perror("fork() failed)");
                        exit(EXIT_FAILURE);

                    default:
                        close(fd[READ]);
                        dup2(fd[WRITE], STDOUT_FILENO);
                        execlp(argv[1], argv[1], NULL);
                }
            }
        } else if (argv[3] == "|") {
            if (argc == 5) {
                error(fd);
                switch(pid = fork()) {
                    case 0:
                        close(fd[WRITE]);
                        dup2(fd[READ], STDIN_FILENO);
                        execlp(argv[4], argv[4], NULL);

                    case -1:
                        perror("fork() failed)");
                        exit(EXIT_FAILURE);

                    default:
                        close(fd[READ]);
                        dup2(fd[WRITE], STDOUT_FILENO);
                        execlp(argv[1], argv[1], argv[2], NULL);
                }
            } else {
                error(fd);
                switch(pid = fork()) {
                    case 0:
                        close(fd[WRITE]);
                        dup2(fd[READ], STDIN_FILENO);
                        execlp(argv[4], argv[4], argv[5], NULL);

                    case -1:
                        perror("fork() failed)");
                        exit(EXIT_FAILURE);

                    default:
                        close(fd[READ]);
                        dup2(fd[WRITE], STDOUT_FILENO);
                        execlp(argv[1], argv[1], argv[2], NULL);
                }
            }
        }
    }
}
