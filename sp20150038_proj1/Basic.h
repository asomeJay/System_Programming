#ifndef __Basic_H__
#define __Basic_H__

#define DIRECTORY 8
#define MAX_STR 255 

typedef struct _linked{
    int cnt;
    char *inst;
    struct _linked *next;
} linked_list;

char **inst_list;
// Basic Function
void init();
void list_make();
int in(char [MAX_STR]);

// Linked List Operation
void list_push(char [MAX_STR]);

// Execution Function 
void dir();
void quit();
void history();
void help();

#endif
