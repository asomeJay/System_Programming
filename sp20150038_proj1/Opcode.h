#ifndef __Opcode_H__
#define __Opcode_H__

#define MAX_INST 10
#define MAX_HASH 20

typedef struct _node{
    char key[MAX_INST];
    int value;
    struct _node *next;
} Node;

Node *hash_table[MAX_HASH];

void opcode_make();

void hash_init();
int hash_function(const char *str);
int hash_find(const char *inkey);
int hash_add(const char *inkey, int value);

void opcode_mnemonic(const char * inkey);
void opcodelist();

#endif
