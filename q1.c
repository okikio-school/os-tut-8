#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "tree.h"
#include "queue.h"
#include "process.h"

int main() {
    process_t data; // Temporary variable to hold the data read from the file.
    // Attempt to open the file containing process information.
    FILE *file = fopen("processes_tree.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "File not found\n");
        exit(EXIT_FAILURE);
    }

    // Root node of the binary tree.
    tree_t* root = NULL;

    // Read each line from the file and insert the process data into a queue.
    while (fscanf(file, "%[^,], %[^,], %d, %d\n", data.parent, data.name, &data.priority, &data.memory) == 4) {
        if (strcmp(data.parent, "NULL") == 0) {
            root = create_node(data);
        } else {
            root = insert_proc(root, data);
        }
    }

    // Close the file after reading the contents.
    fclose(file);
    
    // Print the tree
    printf("Binary Tree Contents:\n");
    print_tree(root, 0);

    // Free the allocated memory for the binary tree.
    free_proc_tree(root);
    return 0;
}
