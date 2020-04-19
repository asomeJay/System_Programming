#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "Assemble.h"

int sym_index;

void assemble_init(){
    int i;
    for (i = 0; i < 200; i++)  {
        sym_table[i].symbol = (char *)malloc(sizeof(char) * SYMBOL);
        sym_table[i].addr = 0;
    }

    opcode[0].symbol = "STL"; opcode[0].op = "14";
    opcode[1].symbol = "LDB"; opcode[0].op = "68";
    opcode[2].symbol = "+JUSB"; opcode[0].op = "48";
    opcode[3].symbol = "LDA"; opcode[0].op = "00";
    opcode[4].symbol = "COMP"; opcode[0].op = "28";
    opcode[5].symbol = "JEQ"; opcode[0].op = "30";
    opcode[6].symbol = "J"; opcode[0].op = "3C";
    opcode[7].symbol = "STA"; opcode[0].op = "0C";
    opcode[8].symbol = "CLEAR"; opcode[0].op = "B4";
    opcode[9].symbol = "+LDT"; opcode[0].op = "74";
    opcode[10].symbol = "TD"; opcode[0].op = "E0";
    opcode[11].symbol = "RD"; opcode[0].op = "D8";
    opcode[12].symbol = "COMPR"; opcode[0].op = "A0";
    opcode[13].symbol = "STCH"; opcode[0].op = "54";
    opcode[14].symbol = "TIXR"; opcode[0].op = "B8";
    opcode[15].symbol = "JLT"; opcode[0].op = "38";
    opcode[16].symbol = "STX"; opcode[0].op = "10";
    opcode[17].symbol = "RSUB"; opcode[0].op = "4C";
    opcode[18].symbol = "LDCH"; opcode[0].op = "50";
    opcode[19].symbol = "WD"; opcode[0].op = "DC";
    opcode[20].symbol = "JLT"; opcode[0].op = "38";
}

void type(char * filename){
    FILE * fp = fopen(filename, "r");
    if(fp == NULL){
        printf("TYPE ERROR\n");
        return;
    }
    char c;
    while((int)(c = fgetc(fp)) != EOF){
        printf("%c", c);
    }
    
    fclose(fp);
    return;
}

void assemble(char * filename){
    int address, line_number, i;
    char list_filename[255], obj_filename[255];

    // 파일을 연다. 문제가 있으면 에러 출력
    FILE * fp = fopen(filename, "r");
    if(fp == NULL){
        printf("assemble error\n");
        return;
    }

    for (i = 0; filename[i] != '.'; i++){
        list_filename[i] = filename[i];
        obj_filename[i] = filename[i];
    }
    list_filename[i] = '\0';
    obj_filename[i] = '\0';

    strcat(list_filename, ".lst");
    strcat(obj_filename, ".obj");

    printf("ASSEMBLE LIST: %s, OBJ : %s\n", list_filename, obj_filename);

    // 파일을 쭈우우욱 읽어들인다. 한 줄씩 읽을 건데 넉넉하게 60개 잡아놓자
    /* 파일이 끝날 때 까지 읽어들여서 .lst 파일과 .obj 파일을 만든다. */
    FILE *f_lst, *f_obj;
    f_lst = fopen(list_filename, "w+");
    f_obj = fopen(obj_filename, "w+");

    if(f_lst == NULL || f_obj == NULL){
        printf("ERROR LST OR OBJ FILE\n");
        return;
    }
    // .asm 파일을 한 줄 씩 읽어들인다. 

    /* PASS 1 : 한 줄씩 읽으면서 각 Symbol의 Location Counter를 만든다.
    => Symbol Table을 만든다. 
    Format 1:  ...  - few used
    Format 2: 2byte - register 
    Format 3: 3byte - standard
    Format 4: 4byte
    */

    address = 0;
    line_number = 0; 
    
    while (1) {
        // Local Variable로 선언함으로써 하나하나 grabage 생각하지 않아도됨
        char src_line[LINE], parsed_line[3][LINE];
        int blank;
        line_number += 5;

        if (fgets(src_line, LINE, fp) == NULL){
            // NULL 인 경우 왠만하면 파일 끝에 다다랐다는 것이므로 while 문을 종료한다.
            break;
        }
        // 소스 파일에서 한 줄 읽었으면 명령어로 바꿔줘야한다.

        blank = parse_line(parsed_line, src_line);
        // |ㅁ|ㅁ|ㅁ| OR |ㅁ|ㅁ| |
        if (strstr(parsed_line[0], ".")){
            fprintf(f_lst, "%7d                %s\n",
            line_number, src_line);
            continue;
        }
        printf("blank : %d\n", blank);
        // START No Symbol
        if (!strcmp(parsed_line[1], "START"))
        {
            int temp = 0;
            int i;
            for (i = 0; parsed_line[2][i] != '\0'; i++)
            {
                temp *= 16;
                temp += (parsed_line[2][i] - '0');
            }

            address = temp;
            printf("START POS : %X \n", address);
            continue;
        }

        // BASE No Symbol
        
        if(!strcmp(parsed_line[0], "BASE")){
            continue;
        }
        printf("blank : %d\n", blank);
        if (blank == 1)  { // No Symbol
            address_increase(&address, parsed_line[0], parsed_line[1]);
        }
        else if (blank == 2)  { // Symbol
            // Push the address and Label
            push_symbol(address, parsed_line[0]);
            address_increase(&address, parsed_line[1], parsed_line[2]);
            
            //obj_code = obj_make(address, parsed_line[1], parsed_line[2]);
            // 형식에 맞춰서 lst 파일에 넣는다.
            //fprintf(f_lst, "%7d %04X %4s %7s %20s %30s ",
            //       line_number, address, parsed_line[0], parsed_line[1], parsed_line[2], obj_code);
            /* 현재 Instruction 종류에 따라 address 증가시킨다.*/
        }
        else  {
            printf("Assemble Parsing ERROR : %s\n", parsed_line[0]);
            return;
        }

    }
    fclose(fp);

    /* 
    PASS2 : Obj 코드를 만든다. 

    */

    fp = fopen(filename, "r");

    if(fp == NULL){
        printf("assemble PASS 2error\n");
        return;
    }

    address = 0;
    line_number = 0; 

    while(1){
        char src_line[LINE], parsed_line[3][LINE];
        int blank;
        line_number += 5;

        if (fgets(src_line, LINE, fp) == NULL){
            // NULL 인 경우 왠만하면 파일 끝에 다다랐다는 것이므로 while 문을 종료한다.
            break;
        }
        // 소스 파일에서 한 줄 읽었으면 명령어로 바꿔줘야한다.
        
        blank = parse_line(parsed_line, src_line);
        // |ㅁ|ㅁ|ㅁ| OR |ㅁ|ㅁ| |
        if (parsed_line[0] == "."){
            fprintf(f_lst, "%7d                %s\n",
            line_number, src_line);
            continue;
        }
        // START - Obj X
        if (!strcmp(parsed_line[0], "COPY"))  {
            fprintf(f_lst, "%7d %04X %4s %7s %20s %30s ",
            line_number, address, parsed_line[0], parsed_line[1], parsed_line[2]," ");
            continue;
        }

        // BASE - Obj X
        if(!strcmp(parsed_line[0], "BASE"))  {
            fprintf(f_lst, "%7d %04X %4s %7s %20s %30s ",
            line_number, address, " ", parsed_line[0], parsed_line[1], " ");
            continue;
        }

        printf("blank : %d\n", blank);
        if (blank == 1)  { // No Symbol
            address_increase(&address, parsed_line[0], parsed_line[1]);
            
        }
        else if (blank == 2)  { // Symbol
            // Push the address and Label
            address_increase(&address, parsed_line[1], parsed_line[2]);
            
            //obj_code = obj_make(address, parsed_line[1], parsed_line[2]);
            // 형식에 맞춰서 lst 파일에 넣는다.
            //fprintf(f_lst, "%7d %04X %4s %7s %20s %30s ",
            //       line_number, address, parsed_line[0], parsed_line[1], parsed_line[2], obj_code);
            /* 현재 Instruction 종류에 따라 address 증가시킨다.*/
        }
        else  {
            printf("Assemble Parsing ERROR\n");
            return;
        }

        fclose(fp);
    }

}

void symbol(){
   /* 정렬하는 부분 */
   /* 알파벳 순으로 정렬하자... */
    int i, j;
    for(i = 0; i < sym_index; i++){
        for(j = i + 1; j < sym_index; j++){
            if(strcmp(sym_table[i].symbol,sym_table[j].symbol) > 0){
              char temp[8];
              int int_temp;
              strcpy(temp, sym_table[i].symbol);
              strcpy(sym_table[i].symbol, sym_table[j].symbol);
              strcpy(sym_table[j].symbol, temp);
 
              int_temp = sym_table[i].addr;
              sym_table[i].addr = sym_table[j].addr;
              sym_table[j].addr = int_temp;
            }
        }
    }
   

   /* 출력하는 부분 */
    /* 출력은 끝에 다다를 때 까지 하면 그만 */ 
    
    for(i = 0; i < sym_index; i++){
        printf("      ");
        printf("%7s  ", sym_table[i].symbol);
        printf("%04d\n", sym_table[i].addr);
    }
    return;
}

int parse_line(char parse[3][LINE], char origin[LINE]){
    int i = 0, j = 0, k = 0, blank = 0;
// 제일 앞이 빈 공간인 거 다 제껴부려~~
    while(origin[i] == ' '){
        i++;
        continue;
    }

    for(; origin[i] != '\0';){ // To the end of String, save the each word.
        if(origin[i] != ' '){
            parse[j][k] = origin[i];
            k++;
            i++;
        }
        else {
            parse[j][k] = '\0';
            blank++;
            j++; 
            k = 0;
            while(origin[i] == ' ' && origin[i] != '\0')
                i++;
        }
    }
    return blank;
}

void push_symbol(int addr, char inst[60]){
    // symbol 넣어주기
    strcpy(sym_table[sym_index].symbol, inst);
    // addr 넣기
    sym_table[sym_index].addr = addr;
    sym_index++;
    return;
    
}

/* 명령어 종류에 따라 address를 증가시킨다. */
/* format3 : opcode (6) nixbpe dis(12)
10진수 4096 -> 16진수 1000 
format4 : opcode 6 nixbpe dis 20.
+ 있으면 format 4이다. 
format2 : op 8, r1 4 r2 4 ( register가 9개 )
*/
void address_increase(int *addr, char inst[LINE], char operand[LINE]){
    if(inst[0] == '+'){
        // 4형식
        (*addr) += 4;   
    }
    else if(strcmp(inst, "BYTE") == 0){
        int i = 0, count = 0;
        char mode = operand[0];

        while(operand[i++] != '\'')
            ;
        while(operand[i] != '\''){
            i++;
            count++;
        }
        if(mode == 'C'){
            (*addr) += count;
        }
        else if(mode == 'X'){
            (*addr) += (count / 2);
        }
    }
    else if(!strcmp(inst, "WORD")){
        (*addr) += 3;
    }
    else if(!strcmp(inst, "RESW")){
        (*addr) += ((int)(stoi(operand)) * 3);
    }
    else if(!strcmp(inst, "RESB")){
        (*addr) += ((int)(stoi(operand)));
    }
    else if(!strcmp(inst, "CLEAR") || !strcmp(inst, "TIXR")){
        (*addr) += 2;
    }
}

/* 00 -> SIC intsruction
01 -> immediate : Value (#) 
10 -> indirect
11 -> simple addressing 

Format 3
OPcode nixbpe displacement(12)
oooooo oooooo oooooooooooo
*/
 

/* 
step 1 : OPCODE
step 2 : nixbpe
    -> simple : n=1, i=1
    -> immediate : n=0, i=1 #
    -> indirect : n=1, i=0 @
    ->
    x = Array
    b = base register
    p = program counter(default)
    e = format 4
step 3 : Displacement or address 
    -> format3 : 12bit
    -> format4 : 20bit : add 0 in front of Locnumber and Displacment 
etc : format 2-> opcode + RegNumber + 0

*/
char * obj_make(int PC, char operation[LINE], char operand[LINE], char * objcode){
// PC already increased(latest)
    int k, disp, next_line;
    char n, i, x, b, p, e;
    char *first, *second, *operands, third[4], fourth[4];

    /* Step 1: Opcode */

    // Geting a Opcode... 
    for (k = 0; k < 21; k++)  {
        if(!strcmp(opcode[k].symbol, operation)){
            operands = opcode[k].op;
            break;
        }
    }

    first = dex_to_bit(operands[0]);
    second = dex_to_bit(operands[1]);
    strcpy(objcode, first);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /* Step 2: nixbpe */
    // Find Symbol... If result is -1, there isn't.
    disp = symbol_find(operand);
    if(operation[0] == '+')  { // format 4 : address : 20bit
        if(operand[0] == '#')  {
            n = '0';
            i = '1';
            x = '0';
            b = '0';
            e = '1';
            if (operand[1] >= '0' && operand[1] <= '9') // Immediate addressing n = 0, i = 1 
                p = '0';
            else  // simple addressing n=0, i=1. 
                p = '1';
        }
        else  {    // simple addressing.
            n = '1';
            i = '1';
            x = '0';
            b = '0';
            p = '1';
            e = '1';
        }
    }
    else if(!strcmp(operation, "CLEAR") || !strcmp(operation, "COMPR")){ // format 2-> opcode + RegNumber + 0
        n = '0';
        i = '0';
        x = '0';
        b = '0';
        p = '0';

        switch(operand[0])  {
        case 'B':
            e = '3';
            break;
        case 'S':
            e = '4';
            break;
        case 'T':
            e = '5';
            break;
        case 'F':
            e = '6';
            break;
        case 'A':
            e = '0';
            break;
        case 'X':
            e = '1';
            break;
        case 'L':
            e = '2';
            break;
        }        
    }
    else { // format 3

        if (operand[0] == '#')  { // immediate addressing n = 0, i = 1;
            n = '0';
            i = '1';
            x = '0';
            b = '0';

            if (operand[1] >= '0' && operand[1] <= '9')
                p = '0';
            else
                p = '1';
            
            e = '0';
        }

        else  { // simple addressing. n = 1, i = 1;
            n = '1';
            i = '1';
            x = '0';
            b = '0';
            p = '1';
            e = '0';

            if (strstr(operand, "BUFFER") != NULL)  { // Buffer Access
                x = '1';
                int distance = symbol_find("BUFFER");
                if (PC - distance >= 4096){
                    b = '1';
                    p = '0';
                }
            }
        }
    }

    second[2] = n;
    second[3] = i;
    third[0] = x;
    third[1] = b;
    third[2] = p;
    third[3] = e;

    strcat(objcode, second);
    strcat(objcode, third);
    ////////////////////////////////////////////////////////////////////////////////////////////

    /* Step 3 : Displacement or Address */

    if(operation[0] == '+') {// format 4
        int kk, temp = 0;
        char *format4_addr;
        format4_addr = (char *)malloc(sizeof(char) * 5);
        for (kk = 0; kk < 5; kk++)
            format4_addr[kk] = '0';

        if(operand[0] == '#')  { // Immediate Addressing n=0, i=1
            if(operand[1] >= '0' && operand[1] <= '9')  {
                for (k = 1; operand[k] != '\0'; k++)  {
                    temp *= 10;
                    temp += (operand[k] - '0');
                }
            }
            else {
                char new_op[LINE];
                for (k = 1; operand[k] != '\0'; k++){
                    new_op[k - 1] = operand[k];
                }
                new_op[k] = '\0';
                temp = symbol_find(new_op);            
            }       
        }

        else 
            temp = symbol_find(operand);

        /* Int Address Value => Char Address Value */
        if (temp >= (int)pow(16, 4.0) ){
            format4_addr[0] = temp / pow(16, 4.0);
            temp -= format4_addr[0];
            format4_addr[0] += '0';
        }
        if(temp >= (int) pow(16, 3.0)){
            format4_addr[1] = temp / pow(16, 3.0);
            temp -= format4_addr[1];
            format4_addr[1] += '0';
        }
        if(temp >= (int) pow(16, 2.0)){
            format4_addr[2] = temp / pow(16, 2.0);
            temp -= format4_addr[2];
            format4_addr[2] += '0';
        }
        if(temp >= (int) pow(16, 1.0)){
            format4_addr[3] = temp / pow(16, 1.0);
            temp -= format4_addr[3];
            format4_addr[3] += '0';
        }
        format4_addr[4] = temp + '0';

        strcat(objcode, format4_addr);
    }

    else if (!strcmp(operation, "CLEAR")){
        strcat(objcode, "0000");
    }
    else if(!strcmp(operation, "COMPR")){ // format 2
        strcat(objcode, "000");
        switch(operand[3]){
        case 'B':
            e = '3';
            break;
        case 'S':
            e = '4';
            break;
        case 'T':
            e = '5';
            break;
        case 'F':
            e = '6';
            break;
        case 'A':
            e = '0';
            break;
        case 'X':
            e = '1';
            break;
        case 'L':
            e = '2';
            break;   
        }
        char tt[2];
        tt[0] = 'e'; tt[1] = '\0';
        strcat(objcode, tt);
    }
    else { // format 3 => 16bit
        int temp = 0;
        char format3_addr[4];

        if(operand[0] == '#')  { //immediate addressing n=0, i=1;
            if(operand[1] >= '0' && operand[1] <= '9'){
                for (k = 1; operand[k] != '\0'; k++)  {
                    temp *= 10;
                    temp += (operand[k] - '0');
                }
            }

            else  {
                char new_op[LINE];
                for (k = 1; operand[k] != '\0'; k++){
                    new_op[k - 1] = operand[k];
                }
                new_op[k] = '\0';
                temp = symbol_find(new_op);     
            }
        }
        else { // simple addressing n =1, i = 1
            temp = symbol_find(operand);
        }

        /* Int Address Value => Char Address Value */
        if (temp >= (int)pow(16, 3.0) ){
            format3_addr[0] = temp / pow(16, 3.0);
            temp -= format3_addr[0];
            format3_addr[0] += '0';
        }
        if(temp >= (int) pow(16, 2.0)){
            format3_addr[1] = temp / pow(16, 2.0);
            temp -= format3_addr[1];
            format3_addr[1] += '0';
        }
        if(temp >= (int) pow(16, 1.0)){
            format3_addr[2] = temp / pow(16, 1.0);
            temp -= format3_addr[2];
            format3_addr[2] += '0';
        }
        format3_addr[3] = temp + '0';
        strcat(objcode, format3_addr);
    }

    printf("%s\n", objcode);
    return objcode;
}

int symbol_find(char operation[LINE]){
    int i;
    for (i = 0; i < sym_index; i++){
        if(!strcmp(operation, sym_table[i].symbol)){
            return sym_table[i].addr;
        }
    }
    return -1;
}

char * dex_to_bit(char fbit){
    switch(fbit){
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'A':
        return "1010";
    case 'B':
        return "1011";
    case 'C':
        return "1100";
    case 'D':
        return "1101";
    case 'E':
        return "1110";
    case 'F':
        return "1111";
    }
    return "0";
}

int stoi(char * target){
    int i, temp = 0, size = strlen(target);

    for (i = 0; i < size; i++){
        temp *= 10;
        temp += (target[i] - '0');
    }

    return temp;
}
