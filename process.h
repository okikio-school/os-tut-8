#include <stdbool.h> // Add this to use bool, true, and false

#ifndef PROCESS_H_
#define PROCESS_H_

#define MAX_NAME_SIZE 256

// Process struct, stores the process state
typedef struct process_t {
    char parent[MAX_NAME_SIZE]; // Name of the parent process.
    char name[MAX_NAME_SIZE];   // Name of the process.
    int priority;               // Priority of the process.
    int memory;                 // Memory in MB used by the process.    
    int pid; // Process ID
    int address; // Memory address index
    int runtime; // Runtime in seconds
    bool suspended; // Indicates if the process is suspended
} process_t;

#endif /* PROCESS_H_ */
