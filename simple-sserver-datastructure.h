#if !defined(SIMPLE_SSERVER_DATASTRUCTURE_H)
#define SIMPLE_SSERVER_DATASTRUCTURE_H
/*
 * simple-sserver-datastructure.h
 *
 * header file for simple-sserver.c detailing the needed the structures for the doubly
 * linked list implementation. Using a doubly linked list, we can gather compilation 
 * and test commands from a given file with are both string arrays dynamically allocated.
 * the Command structure has a head and tail pointer with types Target so each "node" 
 * will be Target type.
 *
 */

typedef struct target {
    int tid;
    char **comp_cmds;
    char **test_cmds;
    struct target *next;
} Target;

typedef struct commands {
    char *name;
    int num_targets;
    Target *head;
    Target *tail;
} Commands;

#endif
