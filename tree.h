#include <stdio.h>
#include <stdlib.h>
#include "process.h"

#ifndef TREE_H_
#define TREE_H_

// Tree struct, stores the process state
typedef struct tree_t {
    process_t process;
    struct tree_t *left;
    struct tree_t *right;
} tree_t;

extern tree_t* create_node(process_t new_proc);
extern tree_t* insert_proc(tree_t* node, process_t new_proc);
extern void print_tree(tree_t* node, int level);
extern void free_proc_tree(tree_t *tree);

#endif /* TREE_H_ */