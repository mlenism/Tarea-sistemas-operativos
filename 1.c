#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define READ  0
#define WRITE 1

// Funcion para separar comandos de una cadena en 2 buffers
void comandos(char* source, char *buf1, char *buf2) {
    int i = 0;
    for (; i < strlen(source); i++) {
        if (source[i] == ' ') {
            i++;
            break;
        }
        buf1[i] = source[i];
    }
    for (int j = 0; i < strlen(source); i++, j++) {
        buf2[j] = source[i];
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int   fd[2];

    /* Si del primer comando obtenemos la cadena "cat 1" */
    char first1[20]; // Aquí se guardaría por ejemplo cat
    char first2[30]; // Aquí se guardaría por ejemplo 1

    /* Si del segundo comando obtenemos la cadena "wc -l" */
    char second1[25]; // Aquí se guardaría por ejemplo wc
    char second2[15]; // Aquí se guardaría por ejemplo -l

    /* Esta parte del codigo se encarga de hacer la separación de comandos */
    comandos(argv[1], first1, first2);
    comandos(argv[2], second1, second2);
  
    if (pipe(fd) == -1) {
        perror("Creating pipe");
        exit(EXIT_FAILURE);
    }

    switch(pid = fork()) {

    case 0:
        // The child process will execute wc.
        // Close the pipe write descriptor.
        close(fd[WRITE]);
        // Redirect STDIN to read from the pipe.
        dup2(fd[READ], STDIN_FILENO);
        // Execute wc
        execlp(second1, second1, second2, NULL);

    case -1:
        perror("fork() failed)");
        exit(EXIT_FAILURE);

    default:
        // The parent process will execute ls.
        // Close the pipe read descriptor.
        close(fd[READ]);
        // Redirect STDOUT to write to the pipe.
        dup2(fd[WRITE], STDOUT_FILENO);
        // Execute ls -l
        execlp(first1, first1, first2, NULL);
    }
}
