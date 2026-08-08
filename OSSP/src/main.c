/*
 * main.c - Command Executor
 *
 * Practical Week 2 - Task 1
 * Demonstrates how a Linux OS executes a command entered by a user:
 *   1. Accept a Linux command as input
 *   2. Create a child process using fork()
 *   3. Execute the command in the child process using exec()
 *   4. Make the parent process wait for the child using wait()
 *   5. Display the PID of both parent and child processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_INPUT   1024
#define MAX_ARGS    64

/* Split the raw command line into an argv[]-style array for execvp(). */
static void tokenize(char *line, char *argv[], int max_args) {
    int argc = 0;
    char *token = strtok(line, " \t\n");

    while (token != NULL && argc < max_args - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;
}

int main(void) {
    char input[MAX_INPUT];
    char *argv[MAX_ARGS];
    pid_t pid, wpid;
    int status;

    printf("Enter a Linux command: ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "No input received.\n");
        return 1;
    }

    /* Strip trailing newline */
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) {
        fprintf(stderr, "Empty command.\n");
        return 1;
    }

    tokenize(input, argv, MAX_ARGS);
    if (argv[0] == NULL) {
        fprintf(stderr, "No command parsed.\n");
        return 1;
    }

    printf("Parent Process\n");
    printf("Child Process\n");

    pid = fork();

    if (pid < 0) {
        /* fork() failed */
        perror("fork failed");
        return 1;

    } else if (pid == 0) {
        /* ---- Child process ---- */
        printf("Child PID : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());
        printf("Executing command: %s\n", input);
        fflush(stdout);

        execvp(argv[0], argv);

        /* execvp only returns on failure */
        fprintf(stderr, "exec failed: %s\n", strerror(errno));
        _exit(127);

    } else {
        /* ---- Parent process ---- */
        printf("Parent PID : %d\n", getpid());
        printf("Child PID : %d\n", pid);

        wpid = wait(&status);

        if (wpid > 0) {
            if (WIFEXITED(status)) {
                printf("Child process completed (exit code %d).\n",
                       WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Child process terminated by signal %d.\n",
                       WTERMSIG(status));
            } else {
                printf("Child process completed.\n");
            }
        }
    }

    return 0;
}
