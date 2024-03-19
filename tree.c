#include <string.h>
#include "tree.h"

/**
 * Creates a new tree node with the given process data.
 * 
 * @return Pointer to the newly created tree node.
 */
extern  tree_t* create_node(process_t new_proc) {
    tree_t* newNode = (tree_t*)malloc(sizeof(tree_t));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    newNode->process = new_proc;
    newNode->left = newNode->right = NULL;
    return newNode;
}

/**
 * Inserts a new node into the tree.
 * 
 * @param tree The tree to insert the new node into.
 * @param process The process data to be stored in the new node.
 * @return Pointer to the newly created tree node.
 */
extern tree_t* insert_proc(tree_t* node, process_t new_proc) {
    if (node == NULL) {
        return create_node(new_proc);
    }
    // Simplified comparison; real-world applications may require more complex logic
    if (strcmp(new_proc.parent, node->process.name) == 0) {
        if (node->left == NULL) {
            node->left = create_node(new_proc);
        } else {
            node->right = create_node(new_proc);
        }
    } else {
        insert_proc(node->left, new_proc);
        insert_proc(node->right, new_proc);
    }
    return node;
}

/**
 * Recursively prints the binary tree, showing each process's name, priority, and memory usage.
 * 
 * @param tree The root of the binary tree to print.
 * @param space The indentation level for pretty printing.
 */

// Print the tree
extern void print_tree(tree_t* node, int level) {
    if (node != NULL) {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("%s (Priority: %d, Memory: %dMB)\n", node->process.name, node->process.priority, node->process.memory);
        print_tree(node->left, level + 1);
        print_tree(node->right, level + 1);
    }
}

/**
 * Frees the memory allocated for the binary tree.
 * 
 * @param tree The root of the binary tree to free.
 */
extern void free_proc_tree(tree_t *tree) {
    if (tree == NULL) {
        return;
    }
    free_proc_tree(tree->left);
    free_proc_tree(tree->right);
    free(tree);
}
