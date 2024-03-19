#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    int status;

    // Forking to create a child process
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process PID: %d\n", getpid());

        // Replace child's image with `./proceess`
        // Execute the pre-compiled program './process'
        execl("./process", "./process", (char *)NULL);

        // execlp only returns on error
        perror("execlp");
        exit(1);
    } else {
        // Parent process
        // Sleep for 5 seconds before sending SIGTSTP
        sleep(5);
        kill(pid, SIGTSTP);

        // Sleep for 10 seconds before resuming child process
        sleep(10);
        kill(pid, SIGCONT);

        // Wait for the child to terminate
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
