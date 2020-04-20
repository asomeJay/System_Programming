#ifndef __Assemble_H__
#define __Assemble_H__

#define SYMBOL 7
#define INSTRUCTION 60
#define LINE 60
#define OBJCODE 60

typedef struct sym_element{
    char * symbol ;
    int addr;
} element;

typedef struct _element{
    char * symbol;
    char * op;
} op_element;

element sym_table[200];
op_element opcode[200];

//int sym_index = 0;
/* filename을 받아서 그 파일을 연다. 
만약 열리지 않으면 에러를 출력하고 종료한다.
열린다면 그 파일을 읽어서 Stdout 한다 */

void type(char * filename);
void assemble(char * filename);
void symbol();
void push_symbol(int addr, char [LINE]);
void assemble_init();
void address_increase(int * addr, char [LINE], char[LINE]);
void obj_make(int, char[LINE], char[LINE], char *);
void dex_to_bit(char *, char);

int stoi(char *);
int symbol_find(char[LINE]);
int parse_line(char[3][LINE], char[LINE]);


#endif
