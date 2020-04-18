#ifndef __Assemble_H__
#define __Assemble_H__

typedef struct sym_element{
    char symbol[7];
    int addr;
} element;

element sym_table[200];
char third[

//int sym_index = 0;
/* filename을 받아서 그 파일을 연다. 
만약 열리지 않으면 에러를 출력하고 종료한다.
열린다면 그 파일을 읽어서 Stdout 한다 */

void type(char * filename);
void assemble(char * filename);
void symbol();
void push_symbol(int addr, char [60]);
int parse_line(char [3][60], char [60]);

#endif
