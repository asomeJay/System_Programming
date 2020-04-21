#ifndef __Assemble_H__
#define __Assemble_H__

#define SYMBOL 7
#define INSTRUCTION 60
#define LINE 60
#define OBJCODE 60
#define MAX_CHAR 255

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

// HOMEWORK //
void type(char * filename);
void assemble(char * filename);
void symbol();
////////////////////////


int push_symbol(int addr, char [LINE]);
//check this instruction is valuable
int instruction_check(char *);

//Init, Setting objcode.
void assemble_init();

// according to instruction, increase address.
void address_increase(int * addr, char [LINE], char[LINE]);
// print assemble code into FILE pointer
void print_assemble(FILE *fp, int addr, int location, char *symbol, char *operation, char *operand, char *objcode);
// make objcode
int obj_make(int, char[LINE], char[LINE], char *, int format);
// parse instruction line by three chunk
void parse_line(char[3][LINE], char[LINE]);

void dex_to_bit(char *, char);
void char_to_dex(int , char *, int *);

char register_to_num(char);
char int_to_dex(int);
char *binary_to_dex(char *);

int stoi(char *);

//find the symbol and return it's address
int symbol_find(char[LINE]);

#endif
