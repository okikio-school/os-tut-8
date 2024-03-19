#ifndef PROCESS_H_
#define PROCESS_H_

#define MAX_NAME_SIZE 256

// Process struct, stores the process state
typedef struct process_t {
    char parent[MAX_NAME_SIZE]; // Name of the parent process.
    char name[MAX_NAME_SIZE];   // Name of the process.
    int priority;               // Priority of the process.
    int memory;                 // Memory in MB used by the process.
} process_t;

#endif /* PROCESS_H_ */
