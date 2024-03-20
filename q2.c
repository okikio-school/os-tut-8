#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "queue.h"
#include "process.h"

#define MAX_PROCESSES 100
#define MAX_NAME_LEN 256
#define MEMORY 1024

queue_t *priority_queue = NULL, *secondary_queue = NULL;
int avail_mem[MEMORY] = {0};

bool is_empty(queue_t *queue) {
    return queue == NULL;
}

// Placeholder for exec_process function
int main() {  
    // Attempt to open the file containing process information.
    FILE *file = fopen("processes_q2.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "File not found\n");
        exit(EXIT_FAILURE);
    }
    
    // `input_process_list` is a temporary list that holds the processes read from the file.
    process_t input_process_list[MAX_PROCESSES];
    process_t data; // Temporary variable to hold the data read from the file.

    // `len` is the length of the data stored in `input_process_list` array from the file.
    int len = 0;

    // Read each line from the file and insert the process data into a queue.
    while (fscanf(file, "%[^,], %d, %d, %d\n", data.name, &data.priority, &data.memory, &data.runtime) == 4) {
        data.pid = 0;
        data.address = 0; // Indicating not yet allocated
        data.suspended = false;
        input_process_list[len++] = data;
    }

    // Close the file after reading the contents.
    fclose(file);
    
    // For efficient memory management, shrink the dispatch list to the number of processes actually read from the file.
    process_t dispatch_list[len];
    for (int i = 0; i < len; i++) {
        dispatch_list[i] = input_process_list[i];
    }
    
    // Sort the dispatch list by arrival time.
    int dispatch_list_len = sizeof(dispatch_list) / sizeof(dispatch_list[0]);

    // Push the processes into the appropriate queues based off of their priority.
    for (int i = 0; i < dispatch_list_len; i ++) {
        process_t *proc = &dispatch_list[i];
        if (proc->priority == 0) {
            push(&priority_queue, proc);
        } else {
            push(&secondary_queue, proc);
        }
    }
    
    int mem_index = 0;
    process_t *current_process = pop(&priority_queue);

    // Priority queue
    while (current_process != NULL) {
        int status;

        for (int i = mem_index; i < mem_index + current_process->memory; i++) {
            // Allocate the memory
            avail_mem[i] = 1;
        }

        current_process->address = mem_index;
        mem_index += current_process->memory;

        // Forking to create a child process
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (pid == 0) {
            current_process->pid = getpid();

            // Child process
            printf("Name: %s, Priority: %d, PID: %d, Address: %d, Runtime: %d\n", current_process->name, current_process->priority, current_process->pid, current_process->memory, current_process->runtime);

            // Replace child's image with `./proceess`
            // Execute the pre-compiled program './process'
            execl("./process", "./process", (char *)NULL);

            // execl only returns on error
            perror("execl");
            exit(1);
        } else {
            // Parent process
            // Sleep for 5 seconds before sending SIGTSTP
            sleep(current_process->runtime);
            kill(pid, SIGTSTP);

            // Wait for the child to terminate
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Child exited with status %d\n", WEXITSTATUS(status));
            }
        }

        for (int i = current_process->address; i < current_process->address + current_process->memory; i++) {
            // Deallocate the memory
            avail_mem[i] = 0;
        }

        // Reset the memory index
        mem_index = current_process->address;
        current_process = pop(&priority_queue);
    }

    // Secondary queue
    current_process = pop(&secondary_queue);
    while (current_process != NULL) {
        int status;
        if ((mem_index - current_process->memory) < MAX_PROCESSES) {
            printf("Insufficient memory for process %s\n", current_process->name);
            push(&secondary_queue, current_process);
            current_process = pop(&secondary_queue);
            continue;
        }

        for (int i = mem_index; i < mem_index + current_process->memory; i++) {
            // Allocate the memory
            avail_mem[i] = 1;
        }
        
        current_process->address = mem_index;
        mem_index += current_process->memory;

        // Forking to create a child process
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (pid == 0) {
            current_process->pid = getpid();

            // Child process
            printf("Name: %s, Priority: %d, PID: %d, Address: %d, Runtime: %d\n", current_process->name, current_process->priority, current_process->pid, current_process->memory, current_process->runtime);

            // Replace child's image with `./proceess`
            // Execute the pre-compiled program './process'
            execl("./process", "./process", (char *)NULL);

            // execl only returns on error
            perror("execl");
            exit(1);
        } else {
            // Parent process
            if (current_process->suspended == true && current_process->pid > 0) {
                kill(pid, SIGCONT);
                current_process->suspended = false;
            }

            // Sleep for 1 seconds before sending SIGTSTP
            sleep(1);
            kill(pid, SIGTSTP);

            current_process->runtime--;
            current_process->pid = pid;
            current_process->suspended = true;

            if (current_process->runtime > 1) {
                push(&secondary_queue, current_process);
            } else {
                sleep(current_process->runtime);

                // Wait for the child to terminate
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    printf("Child exited with status %d\n", WEXITSTATUS(status));
                }
            }
        }

        for (int i = current_process->address; i < current_process->address + current_process->memory; i++) {
            // Deallocate the memory
            avail_mem[i] = 0;
        }

        current_process = pop(&secondary_queue);
    }

    return 0;
}
