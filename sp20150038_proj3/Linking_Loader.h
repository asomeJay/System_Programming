#ifndef __LINKING_LOADER_H__
#define __LINKING_LOADER_H__

#include <stdio.h>
#include "Assemble.h"
#include "Memory.h"

#define REG_COUNT 9
#define CANT_FIND -1
#define ERROR -1
#define MAX_RECORD 255

element symbol_table[200];

int registers[REG_COUNT];
int symbol_index;

void run();
void loader(FILE **);

int link_loader_pass1(FILE **);
int link_loader_pass2(FILE **);
int is_control_section(char *);
int is_estab(char *);
int record_check(char);

#endif // __LINKING_H__