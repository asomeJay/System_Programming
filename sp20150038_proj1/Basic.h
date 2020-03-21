#ifndef __Basic_H__
#define __Basic_H__

#define DIRECTORY 8

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _linked{
    int cnt;
    char *inst;
    struct _linked *next;
} linked_list;

char **inst_list;

// Basic Function
void init();
void list_make();
int in(char [20]);

// Linked List Operation
void list_push(char [20]);

// Execution Function 
void dir();
void quit();
void history();
void help();

#endif
