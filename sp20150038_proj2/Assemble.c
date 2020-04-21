#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "Assemble.h"

int sym_index, last_address, op_index ;
int base;

void assemble_init()  {
    int i, k;
    for (i = 0; i < 200; i++)  {
        sym_table[i].symbol = (char *)malloc(sizeof(char) * SYMBOL);
        sym_table[i].addr = 0;
        opcode[i].op = (char *)malloc(sizeof(char) * SYMBOL);
        opcode[i].symbol = (char *)malloc(sizeof(char) * SYMBOL);
    }

    FILE *fps = fopen("opcode.txt", "r");
    if(fps == NULL){
        printf("ERROR ASSEMBLE INIT\n");
        return;
    }
    char opc[255];
    k = 0;
    
    while ( !feof(fps) )  {
        fscanf(fps, "%s %s %s\n", opcode[k].op, opcode[k].symbol, opc);
        k++;
    }
    op_index = k;
    fclose(fps);

    /*    opcode[0].symbol = "STL";
    opcode[0].op = "14";
    opcode[1].symbol = "LDB"; opcode[1].op = "68";
    opcode[2].symbol = "JSUB"; opcode[2].op = "48";
    opcode[3].symbol = "LDA"; opcode[3].op = "00";
    opcode[4].symbol = "COMP"; opcode[4].op = "28";
    opcode[5].symbol = "JEQ"; opcode[5].op = "30";
    opcode[6].symbol = "J"; opcode[6].op = "3C";
    opcode[7].symbol = "STA"; opcode[7].op = "0C";
    opcode[8].symbol = "CLEAR"; opcode[8].op = "B4";
    opcode[9].symbol = "LDT"; opcode[9].op = "74";
    opcode[10].symbol = "TD"; opcode[10].op = "E0";
    opcode[11].symbol = "RD"; opcode[11].op = "D8";
    opcode[12].symbol = "COMPR"; opcode[12].op = "A0";
    opcode[13].symbol = "STCH"; opcode[13].op = "54";
    opcode[14].symbol = "TIXR"; opcode[14].op = "B8";
    opcode[15].symbol = "JLT"; opcode[15].op = "38";
    opcode[16].symbol = "STX"; opcode[16].op = "10";
    opcode[17].symbol = "RSUB"; opcode[17].op = "4C";
    opcode[18].symbol = "LDCH"; opcode[18].op = "50";
    opcode[19].symbol = "WD"; opcode[19].op = "DC";*/
}

/*매개변수로 들어온 파일을 열어서 그 내용을 출력합니다. */
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

/* 입력된 filename을 assemble 한다. */
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

    // 파일을 쭈우우욱 읽어들인다. 한 줄씩 읽을 건데 넉넉하게 60개 잡아놓자
    /* 파일이 끝날 때 까지 읽어들여서 .lst 파일과 .obj 파일을 만든다. */
    FILE *f_lst = fopen((const char *)list_filename, "w+");
    FILE *f_obj = fopen((const char *)obj_filename, "w+");
    
    if (f_lst == NULL || f_obj == NULL)  {
        fclose(f_lst);
        fclose(f_obj);
        remove(list_filename);
        remove(obj_filename);
        printf("ERROR LST OR OBJ FILE\n");
        return;
    }
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

        line_number += 5;

        if (fgets(src_line, LINE, fp) == NULL){
            // NULL 인 경우 왠만하면 파일 끝에 다다랐다는 것이므로 while 문을 종료한다.
            break;
        }
        // 소스 파일에서 한 줄 읽었으면 명령어로 바꿔줘야한다.

        if (strstr(src_line, "."))  
            continue;
        
        parse_line(parsed_line, src_line);  // |ㅁ|ㅁ|ㅁ| OR |ㅁ|ㅁ| |

        if (parsed_line[0][0] == 'N')  {   // No Symbol => No need to into
            if (parsed_line[1][0] == '+') {// format4
            /* Format 4는 좀 특이하게 생겼으므로 특이 취급을 해준다. */
                address += 4;

                char for_check[10];
                format4_plus_delete(for_check, parsed_line[1]);

                if (instruction_check(for_check) == -1)  {
                    // 에러가 터지면 어디서 에러가 났는지 보고하고
                    // lst 파일과 obj 파일을 닫고, 이미 만든 건 삭제한다. 
                    printf("%s\n", for_check);
                    printf("ERROR : %d : %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }

                continue;
            }
            else {  // '+' 가 아니다!
                if(instruction_check(parsed_line[1]) == -1)  {
                    printf("ERROR : %d : %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }
                if (!strcmp(parsed_line[1], "BASE"))
                continue;
            else if (!strcmp(parsed_line[1], "END"))
                break;
            else
                address_increase(&address, parsed_line[1], parsed_line[2]);
            }
        }
        else  { // Yes Symbol => need to Symbolize. 
            if (parsed_line[1][0] == '+') {// format4
            /* Format 4는 좀 특이하게 생겼으므로 특이 취급을 해준다. */

                char for_check[10];
                format4_plus_delete(for_check, parsed_line[1]);

                if (instruction_check(for_check) == -1)  {
                    printf("ERROR : %d : %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }
            }
            else {
                if(instruction_check(parsed_line[1]) == -1){
                    printf("ERROR : %d : %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }
            }
            if (!strcmp(parsed_line[1], "START"))  {
                int i, temp = 0;

                for (i = 0; parsed_line[2][i] != '\0' && parsed_line[2][i] != '\n'; i++)  {
                    temp *= 16;
                    temp += (parsed_line[2][i] - '0');                
                }
                address = temp;
            }
            else {
                if(parsed_line[0][0] >= 'A' && parsed_line[0][0] <= 'Z'){
                    // -1이 나오면 중복 삽입이므로 오류! 

                    if(push_symbol(address, parsed_line[0]) == -1){
                        printf("ERROR at %d LINE\n", line_number);
                        printf("%s\n", src_line);
                    }
                }
                address_increase(&address, parsed_line[1], parsed_line[2]);
            }
        }
    }
    fclose(fp);

    /////////////////////////////////////////////////////////////////////////////////

    /* 
    PASS2 : Although sleep come... but We must make OBJCODE!!!!! 
    WE finally enter the final line...!
    */

    fp = fopen(filename, "r");

    if(fp == NULL){
        printf("assemble PASS 2 error : We cant open the file\n");
        return;
    }

    // .asm 파일을 한 줄 씩 읽어들인다.
    last_address = address; 
    address = 0;
    line_number = 0; 
    char ts[2], objfile[255], modifys[32768], now[12];
    objfile[0] = '\0';
    int start_address, rs;
    ts[0] = ' ';
    ts[1] = '\0';
    now[0] = '\0';

    while (1)  {
        char src_line[LINE], parsed_line[3][LINE], obj[INSTRUCTION];

        line_number += 5;

        if (fgets(src_line, LINE, fp) == NULL)  
            // NULL 인 경우 왠만하면 파일 끝에 다다랐다는 것이므로 while 문을 종료한다.
            break;
        
        // 소스 파일에서 한 줄 읽었으면 명령어로 바꿔줘야한다.
        if (strstr(src_line, "."))  {
            fprintf(f_lst, "%-4d     %s", line_number, src_line);
            continue;
        }

        parse_line(parsed_line, src_line);
        // |ㅁ|ㅁ|ㅁ| OR |ㅁ|ㅁ| |
        if (parsed_line[0][0] == 'N')
        {                                   // No Symbol => No need to into
            if(parsed_line[1][0] == '+')  { // format4
                char t_inst[20];
                format4_plus_delete(t_inst, parsed_line[1]);

                if(obj_make(address, t_inst, parsed_line[2], obj, 4) == -1){
                    printf("ERROR %d %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                };
                if(!(parsed_line[2][0] == '#' && (parsed_line[2][1] >= '0' && parsed_line[2][1] <= '9'))){
                    modify_relocation(now, address);
                    strcat(modifys, now);
                }
                    
                print_assemble(f_lst, line_number, address, " ", parsed_line[1], parsed_line[2], obj);
                address += 4;
            }

            else if(!strcmp(parsed_line[1], "BASE"))  { // BASE No OBJ code 
                base = symbol_find(parsed_line[2]);
                if(base == -1){
                    printf("%s ERROR\n", src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }
                fprintf(f_lst, "%-5d %4s     %-6s   %-7s  %-10s  %-10s\n",
                line_number, ts, ts, parsed_line[1], parsed_line[2], ts);
            }
            else if(!strcmp(parsed_line[1], "END")){
                fprintf(f_obj, "%s", modifys);
                fprintf(f_obj, "E%06X\n", rs);
                fprintf(f_lst, "%-5d %4s     %-6s   %-7s  %-10s  %-10s\n",
                    line_number, ts, ts, parsed_line[1], parsed_line[2], ts);
                break;
            }

            else {
                if(obj_make(address, parsed_line[1], parsed_line[2], obj, 3) == -1){
                    printf("ERROR %d %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }
                print_assemble(f_lst, line_number, address, " ", parsed_line[1], parsed_line[2], obj);
                address_increase(&address, parsed_line[1], parsed_line[2]);
            }
        }
        else  {  // YES Symbol ! 
            if (!strcmp(parsed_line[1], "START"))  {
                start_address = address;
                rs = address;
                fprintf(f_obj, "H%-6s %06X%06X\n", parsed_line[0], address, last_address);
                print_assemble(f_lst, line_number, address, parsed_line[0], parsed_line[1], parsed_line[2], " ");
                continue;
            }
            else if(parsed_line[1][0] == '+')  { // format 4
                char t_inst[20];

                format4_plus_delete(t_inst, parsed_line[1]);

                if (obj_make(address, t_inst, parsed_line[2], obj, 4) == -1)  {
                    printf("ERROR %d %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                }
                if(!(parsed_line[2][0] == '#' && (parsed_line[2][1] >= '0' && parsed_line[2][1] <= '9'))){
                    modify_relocation(now, address);
                    strcat(modifys, now);
                }

                print_assemble(f_lst, line_number, address, parsed_line[0], parsed_line[1], parsed_line[2], obj);
                address += 4;
            }
            else {
                if(!strcmp(parsed_line[1], "BYTE") || !strcmp(parsed_line[1], "RESW") 
                || !strcmp(parsed_line[1], "RESB") || !strcmp(parsed_line[1], "WORD")){
                    if((int)strlen(objfile) > 0)  {
                        fprintf(f_obj, "T%06X %02X %s\n", start_address, (int)strlen(objfile) ,objfile);
                        start_address = address;
                        strcpy(objfile, "");
                    }
                }
                if(obj_make(address, parsed_line[1], parsed_line[2], obj, 3) == -1){
                    printf("ERROR %d %s\n", line_number, src_line);
                    fclose(f_lst);
                    fclose(f_obj);
                    remove(list_filename);
                    remove(obj_filename);
                    return;
                } // obj code make
                print_assemble(f_lst, line_number, address, parsed_line[0], parsed_line[1], parsed_line[2], obj);
                address_increase(&address, parsed_line[1], parsed_line[2]);
            }
        }

        if(!strcmp(parsed_line[1], "BYTE") || !strcmp(parsed_line[1], "RESW") 
        || !strcmp(parsed_line[1], "RESB") || !strcmp(parsed_line[1], "WORD"))
            continue;
        
        strcat(objfile, obj);

        if((int)strlen(objfile) > OBJECT_TEXT_LENGTH){ // Dont over Mine. 
            fprintf(f_obj, "T%06X %02X %s\n", start_address, (int)strlen(objfile) ,objfile);
            start_address = address;
            strcpy(objfile, "");
        }
    }
    fclose(fp);
    fclose(f_lst);
    fclose(f_obj);

    printf("SUCCESSFULLY ASSEMBLED!\n");
}

void symbol()  {
   /* 정렬하는 부분 */
   /* 알파벳 순으로 정렬하자... */

   // Ordering by alphabetical order
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
        printf("        "); // space 8
        printf("%-7s  ", sym_table[i].symbol);
        printf("%04X\n", sym_table[i].addr);
    }
    return;
}

void parse_line(char parse[3][LINE], char origin[LINE]){
    int i = 0, j = 0;

    if(origin[0] == ' ')  { // No Symbol => 1 or 2 chunk :)
    //If we don't have operand... then we get 'N'

        parse[0][0] = 'N';
        parse[0][1] = '\0';
        parse[2][0] = 'N';
        parse[2][1] = '\0';

        while (origin[i] == ' ')
            i++;
        while(origin[i] != ' ' && origin[i] != '\n'){
            parse[1][j] = origin[i];
            j++;
            i++;
        }
        parse[1][j] = '\0';  //instruction part

        while (origin[i] == ' ')
            i++;

        if (origin[i] == '\n' || origin[i] == '\0')  {
            parse[2][0] = 'N';
            parse[2][1] = '\0';
        }
        else  {
            j = 0;
            while (origin[i] != '\n' && origin[i] != '\0')
            {
                parse[2][j] = origin[i];
                j++;
                i++;
            }
            parse[2][j] = '\0';
        }
    }
    else  {  // Yes Symbol -> 3 chunk! 
        while(origin[i] != ' '){
            parse[0][j++] = origin[i];
            i++;
        }
        parse[0][j] = '\0'; //symbol part

        j = 0;
        while (origin[i] == ' ')
            i++;
        while(origin[i] != ' '){
            parse[1][j++] = origin[i];
            i++;
        }

        parse[1][j] = '\0'; // instruction part

        j = 0;
        while(origin[i] == ' ')
            i++;
        while(origin[i] != '\n'){
            parse[2][j++] = origin[i];
            i++;
        }

        parse[2][j] = '\0'; // operand part
    }
    return;
}

int push_symbol(int addr, char inst[60]){
    int i;
    for (i = 0; i < sym_index; i++){
        if(!strcmp(sym_table[i].symbol, inst)){
            return -1;
        }
    }
    // symbol 넣어주기
    strcpy(sym_table[sym_index].symbol, inst);
    // addr 넣기
    sym_table[sym_index].addr = addr;
    sym_index++;
    return 1;
    
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
        (*addr) += 4;
    }
    else if(!strcmp(inst, "BYTE")){
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
    else if(!strcmp(inst, "CLEAR") || !strcmp(inst, "COMPR") || !strcmp(inst, "ADDR") || 
    !strcmp(inst, "DIVR") || !strcmp(inst, "MULR") || !strcmp(inst, "TIXR"))   {
        (*addr) += 2;
    }

    else {
        (*addr) += 3;
    }
}
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

/*object code를 만드는 함수입니다. */ 
int obj_make(int PC, char operation[LINE], char operand[LINE], char * objcode, int format){
// PC already increased(latest)
    int k, next_line;
    char n, i, x, b, p, e;
    char first[5], second[5], *operands, third[5];
    next_line = PC;

    /* Handle Constant */
    if(!strcmp(operation, "RESW") || !strcmp(operation, "RESB")){
        strcpy(objcode, ""); // 얘들은 objcode가 없습니다. 
        return 1;
    }

    if (!strcmp(operation, "BYTE"))
    { // 상수는 특별하게 처리해주겠어 
        char new_op[MAX_CHAR], resulted[MAX_CHAR];
        int index = 0;

        if (operand[0] == 'C')  { // byte
            for (k = 2; operand[k] != '\''; k++)  
                new_op[k - 2] = operand[k];
            new_op[k - 2] = '\0';

            for (k = 0; k < strlen(new_op); k++)  {
                char_to_dex(new_op[k],resulted, &index);
            }

            strcpy(objcode, resulted);
            return 1;
        }

        if(operand[0] == 'X'){
            for (k = 2; operand[k] != '\''; k++)  
                new_op[k - 2] = operand[k];
            
            new_op[k-2] = '\0';
            strcpy(objcode, new_op);
            return 1;
        }
    }
/////////////////////////////////////////////////////////////////////////

    address_increase(&next_line, operation, operand);
    // update next_line  for pc relative 

    /* Step 1: Opcode */

    // Geting a Opcode... 
    for (k = 0; k < op_index; k++)  {
        if(!strcmp(opcode[k].symbol, operation)){
            operands = opcode[k].op;
            break;
        }
    }

    dex_to_bit(first, operands[0]);
    dex_to_bit(second, operands[1]);
    strcpy(objcode, binary_to_dex(first));
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /* Step 2: nixbpe */
    // Find Symbol... If result is -1, there isn't.
    if(!strcmp(operation, "RSUB")){
        n = '1';
        i = '1';
        x = '0';
        b = '0';
        p = '0';
        e = '0';
    }
    else if (format == 4)  { // format 4 : address : 20bit
        if(operand[0] == '#')  {
            n = '0';
            i = '1';
            x = '0';
            b = '0';
            p = '0';
            e = '1';
            /*if (operand[1] >= '0' && operand[1] <= '9') // Immediate addressing n = 0, i = 1 
                p = '0';
            else  // simple addressing n=0, i=1. 
                p = '1';*/
        }
        else  {    // simple addressing.
            n = '1';
            i = '1';
            x = '0';
            b = '0';
            p = '0';
            e = '1';
        }
    }
    else if(!strcmp(operation, "CLEAR") || !strcmp(operation, "COMPR") || !strcmp(operation, "TIXR")
    || !strcmp(operation, "RSUB")) { // format 2-> opcode + RegNumber + 0
        n = '0';
        i = '0';

        x = '0';
        b = '0';
        p = '0';
        e = '0';
        switch (operand[0]) {
        case 'B':
            p = '1';
            e = '1';
            break;
        case 'S':
            b = '1';
            break;
        case 'T':
            b = '1';
            e = '1';
            break;
        case 'F':
            b = '1';
            p = '1';
            break;
        case 'A':
            e = '0';
            break;
        case 'X':
            e = '1';
            break;
        case 'L':
            p = '1';
            break;
        }
    }
    else if(!strcmp(operation, "STCH") || !strcmp(operation,"LDCH")){
        n = '1';
        i = '1';
        x = '0';
        
        if(strstr(operand, "BUFFER")){
            int distance = symbol_find("BUFFER");
            if(distance == -1){

                return -1;
            }

            if(distance < next_line){
                b = '1';
                p = '0';
            }
            else {
                b = '0';
                p = '1';
            }

            x = '1';
        }
        e = '0';
    }

    else { // format 3
        if(operand[0] == '@')  { // indirect addressing n = 1, i = 0;
            n = '1';
            i = '0';
            x = '0';
            b = '0';
            p = '1';
            e = '0';
        }
        else if (operand[0] == '#')  { // immediate addressing n = 0, i = 1;
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

            int distance = symbol_find(operand);
            if(distance == -1){
                return -1;
            }
            if (PC - distance >= 4096){
                b = '1';
                p = '0';
            }
            if (strcmp(operand, "BUFFER") && strstr(operand, "BUFFER"))  { // Buffer Access
                x = '1';   
            }
        }
    }

    second[2] = n;
    second[3] = i;
    second[4] = '\0';

    third[0] = x;
    third[1] = b;
    third[2] = p;
    third[3] = e;
    third[4] = '\0';

    strcat(objcode, binary_to_dex(second));
    strcat(objcode, binary_to_dex(third));
    
    ////////////////////////////////////////////////////////////////////////////////////////////

    /* Step 3 : Displacement or Address */

    if (format == 4)  { // format 4
        int kk, temp = 0;
        char format4_addr[7];

        for (kk = 0; kk < 6; kk++)
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
                new_op[k - 1] = '\0';
                temp = symbol_find(new_op);            
            }       
        }

        else 
            temp = symbol_find(operand);

        if(temp == -1)
            return -1;

        if (p == 1)
            temp -= next_line;
        /* Int Address Value => Char Address Value */
        formatting(format4_addr, 4, temp);        
        strcat(objcode, format4_addr);
    }
    else if(!strcmp(operation, "RSUB"))
        strcat(objcode, "000");
    else if (!strcmp(operation, "CLEAR") || !strcmp(operation, "TIXR"))
        strcat(objcode, "0");
    else if(!strcmp(operation, "COMPR")){ // format 2
        int tts = 0;
        while (tts < strlen(operand) && operand[tts] != ',')
            tts++;
        tts++;
        while(operand[tts] <= 'A' || operand[tts] >= 'Z')
            tts++;

        e = register_to_num(operand[tts]);
        char tt[2];
        tt[0] = e; tt[1] = '\0';
        strcat(objcode, tt);
    }
    else if(!strcmp(operation, "STCH") || !strcmp(operation, "LDCH")){
        char format3_addr[4];
        int dist, temp;

        for (k = 0; k < 4; k++){
            format3_addr[k] = '0';
        }
            
        if(x == '1'){
            dist = symbol_find("BUFFER");
            if(dist == -1)
                return -1;
        }

        if (b == '1')  {       

            if ( dist < base ){
                dist += (int)(pow(16, 3));
            }

            dist -= base;
            temp = dist;
        }
        else if(p == '1'){
            if(next_line > dist){
                dist += (int)(pow(16, 3));
            }
            dist -= next_line;
            temp = dist;
        }
        formatting(format3_addr, 2, temp);
        strcat(objcode, format3_addr);
    }
    else { // format 3 => 12bit
        int temp = 0, dist;
        char format3_addr[4];

        for (k = 0; k < 4; k++)
            format3_addr[k] = '0';

        if (operand[0] == '@')  {
            char new_op[LINE];
            for (k = 1; operand[k] != '\0' && operand[k] != '\n'; k++)  {
                new_op[k - 1] = operand[k];
            }
            new_op[k] = '\0';

            temp = symbol_find(new_op);
        }

        else if (operand[0] == '#')  { //immediate addressing n=0, i=1;
            // To decimal.
            if (operand[1] >= '0' && operand[1] <= '9')  {
                for (k = 1; operand[k] != '\0' && operand[k] != '\n'; k++)  {
                    temp *= 10;
                    temp += (operand[k] - '0');
                }
            }

            else  {

                char new_op[LINE];
                for (k = 1; operand[k] != '\0' && operand[k] != '\n'; k++)  {
                    new_op[k - 1] = operand[k];
                }
                new_op[k-1] = '\0';
                temp = symbol_find(new_op);
            }
        }
        else if(b == '1'){
            dist = symbol_find(operand);
            if(dist == -1)
                return -1;
            dist -= base;
            temp = dist;
        }
        else   // simple addressing n =1, i = 1
            temp = symbol_find(operand);
        
        if(temp == -1){
            return -1;
        }

        if(p == '1'){ // pc Relative
            if (temp < next_line)  {
                temp += (int)(pow(16, 3));
            }
            temp -= next_line;
        }
        formatting(format3_addr, 2, temp);
        strcat(objcode, format3_addr);
    }
    return 1;
}

/* Symbol table에서 symbol을 찾아서 그 address를 return 한다 */
int symbol_find(char operation[LINE]){
    int i;
    for (i = 0; i < sym_index; i++){
        if(!strcmp(operation, sym_table[i].symbol)){
            return sym_table[i].addr;
        }
    }
    return -1;
}

/* 매개변수로 들어온 fbit는 16진수 형식입니다. 이걸 4bit binary digit으로 변환합니다. */
void dex_to_bit(char * dest, char fbit){
    switch(fbit){
    case '0':
        dest[0] = '0';
        dest[1] = '0';
        dest[2] = '0';
        dest[3] = '0';
        break;
    case '1':
        dest[0] = '0';
        dest[1] = '0';
        dest[2] = '0';
        dest[3] = '1';
        break;
    case '2':
        dest[0] = '0';
        dest[1] = '0';
        dest[2] = '1';
        dest[3] = '0';
        break;
    case '3':
        dest[0] = '0';
        dest[1] = '0';
        dest[2] = '1';
        dest[3] = '1';
        break;
    case '4':
        dest[0] = '0';
        dest[1] = '1';
        dest[2] = '0';
        dest[3] = '0';
        break;
    case '5':
        dest[0] = '0';
        dest[1] = '1';
        dest[2] = '0';
        dest[3] = '1';
        break;
    case '6':
        dest[0] = '0';
        dest[1] = '1';
        dest[2] = '1';
        dest[3] = '0';
        break;
    case '7':
        dest[0] = '0';
        dest[1] = '1';
        dest[2] = '1';
        dest[3] = '1';
        break;
    case '8':
        dest[0] = '1';
        dest[1] = '0';
        dest[2] = '0';
        dest[3] = '0';
        break;
    case '9':
        dest[0] = '1';
        dest[1] = '0';
        dest[2] = '0';
        dest[3] = '1';
        break;
    case 'A':
        dest[0] = '1';
        dest[1] = '0';
        dest[2] = '1';
        dest[3] = '0';
        break;
    case 'B':
        dest[0] = '1';
        dest[1] = '0';
        dest[2] = '1';
        dest[3] = '1';
        break;
    case 'C':
        dest[0] = '1';
        dest[1] = '1';
        dest[2] = '0';
        dest[3] = '0';
        break;
    case 'D':
        dest[0] = '1';
        dest[1] = '1';
        dest[2] = '0';
        dest[3] = '1';
        break;
    case 'E':
        dest[0] = '1';
        dest[1] = '1';
        dest[2] = '1';
        dest[3] = '0';
        break;
    case 'F':
        dest[0] = '1';
        dest[1] = '1';
        dest[2] = '1';
        dest[3] = '1';
        break;
    }
    dest[4] = '\0';
    return;
}

/*우리가 아는 그 stoi 맞습니다... C는 왜 지원하지 않느냐! 
지원하지 않길래 제가 만들었습니다. string으로 된 정수를 integer로 바꿉니다. */
int stoi(char * target){
    int i, temp = 0, size = strlen(target);

    for (i = 0; i < size; i++){
        temp *= 10;
        temp += (target[i] - '0');
    }

    return temp;
}

/* binary -> Dex change! */
char* binary_to_dex(char * binary){
    int i, digit, temp = 0;
    char *result;

    if (strlen(binary) == 5){
        digit = 16;
        result = (char *)malloc(sizeof(char) * 3);
    }
    else if(strlen(binary) == 4){
        digit = 8;
        result = (char *)malloc(sizeof(char) * 2);
    }
    else {
        printf("strlen : %d\n", (int)strlen(binary));
        printf("s : %s\n", binary);
        return "ERROR AT BINARY_TO_DEX";
    }

    for (i = 0; i < (int)strlen(binary); i++, digit /= 2){
        if(binary[i] == '1') {
            temp += digit;
        }
    }

    if( temp >= 16){
        result[0] = temp / 16;
        temp -= (result[0] * (temp / 16));
        result[1] = temp;

        if(result[0] >= 10){
            result[0] -= 10;
            result[0] += 'A';
        }
        else {
            result[0] += '0';
        }
        if(result[1] >= 10){
            result[1] -= 10;
            result[1] += 'A';
        }
        else{
            result[1] += '0';
        }

        result[2] = '\0';
    }

    else{
        if(temp >= 10){
            result[0] = temp - 10 + 'A';
        }
        else
            result[0] = temp + '0';
        result[1] = '\0';
    }
    return result;
}

/* 10진수 정수를 16진수로 변환한다. */
char int_to_dex(int target){
    if(target < 10){
        target += '0';
    }
    else {
        target -= 10;
        target += 'A';
    }
    return target;
}

void print_assemble(FILE * fp, int addr, int location, char *symbol, char *operation, char *operand, char *objcode){
    fprintf(fp, "%-5d %04X     %-6s   %-7s  %-10s  %-10s\n",
            addr, location, symbol, operation, operand, objcode);
}

/* 이름은 마치 character 값을 변환하는 것 같지만, 실제로는 입력된 t를 두 자리 16진수 비트로 바꿈 */
void char_to_dex(int t, char *temp, int *index){
    temp[(*index)++] = (int)t / 16;
    if(temp[(*index)-1] >= 10){
        temp[(*index) - 1] -= 10;
        temp[(*index) - 1] += 'A';
    }
    else {
        temp[(*index) - 1] += '0';
    }

    temp[(*index)++] = (int)t % 16;
    if(temp[(*index)-1] >= 10){
        temp[(*index) - 1] -= 10;
        temp[(*index) - 1] += 'A';
    }
    else {
        temp[(*index) - 1] += '0';
    }

    temp[(*index)] = '\0';
    return;
}

char register_to_num(char target){
    char e;
    switch (target)  {
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
    return e;
}

int instruction_check(char *inst){
    int i;
    for (i = 0; i < op_index; i++){
        if(!strcmp(inst, opcode[i].symbol))
            return 1;
    }
    if(!strcmp(inst, "BASE") || !strcmp(inst, "BYTE") || !strcmp(inst, "RESW") || !strcmp(inst, "RESB") || 
        !strcmp(inst, "WORD") || !strcmp(inst, "START") || !strcmp(inst, "END"))
        return 1;

    return -1;
}

/* 위에서 dest만 선언해서 넘겨주면 여기서 알아서 M을 만들어준다. */

void modify_relocation(char * dest, int addr)  {
    dest[0] = 'M';

    int i;
    double digit = 5.0;

    for(i = 1; i < 8; i++)
        dest[i] = '0';
    dest[8] = '5';
    dest[9] = '\n';
    dest[10] = '\0';
    addr++;

    i = 1;

    while (digit > 0)  {
        if  (addr >= (int)pow(16, digit)){
            dest[i] = int_to_dex(addr / (int)pow(16, digit));
            addr -= ((addr / (int)pow(16,digit)) * (pow(16,digit)));
        }
        digit--;
        i++;
    }
    dest[i++] = int_to_dex(addr);
    return;
}

/* 주어진 포맷에 맞춰서 digit은 자릿수, target은 16진수로 바꾸려는 integer */

void formatting(char * format, int digit, int target) {
/* Int Address Value => Char Address Value */
    int i = 0;
    while (digit > 0)  {
        if(target >= (int)pow(16, digit))  {
            format[i] = target / (int)pow(16, digit);
            target -= (format[i] * pow(16, digit));
            format[i] = int_to_dex(format[i]);
        }

        i++;
        digit--;
    }
        format[i++] = int_to_dex(target);
        format[i] = '\0';
}

void format4_plus_delete(char *dest, char *src){
    int i;
    for (i = 1; i <strlen(src); i++)  
        dest[i-1] = src[i];
    
    dest[i - 1] = '\0';
}
