/**********************************
 * JaeWon KIM's Project 1       ***
 * Plz Don't Copy me            ***
 * I'm Junior of Junior         ***
 * *******************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Basic.h"
#include "Memory.h"
#include "Opcode.h"
#include "20150038.h"
#include "Assemble.h"

int main(void)
{
    init();     // 초기화 작업을 합니다.
    assemble_init();
    opcode_make(); // 프로그램이 시작될 때 opcode list를 만들어줍니다.

    while (1) // quit을 받을 때까지 계속 명령어를 받습니다. 
    {
        int blank;
        char inst_input[MAX_STR];

        printf("sicsim> "); // 계속 출력하는 것
        blank = in(inst_input);    // inst_input으로 명령 받고 명령어에 빈칸 몇 갠지 return 받는다.
        operation(inst_input, blank); // 받은 명령어를 통해 명령을 수행한다. 
    }

    return 0;
}

/* 명령어 들어온 걸로 실제 그 함수를 실행한다. 
실행할 때마다 history list에 저장한다.*/

// list_push는 history에 추가하려고 넣었다. 
void operation(char inst_input[MAX_STR], int blank){
    if(blank == 0){ // help 처럼 빈칸이 하나도 없는 명령어!
        if(!strcmp(inst_input, "dir") || !strcmp(inst_input, "d")){
            list_push(inst_input);
            dir();
        }
        else if (!strcmp(inst_input, "dump") || !strcmp(inst_input, "du")){
            list_push(inst_input);
            dump(-1, -1, 0);
        }
        else if(!strcmp(inst_input, "help") || !strcmp(inst_input, "h")){
            list_push(inst_input);
            help();
        }
        else if(!strcmp(inst_input, "history") || !strcmp(inst_input, "hi")){ // history
            list_push(inst_input);
            history();
        }
        else if(!strcmp(inst_input, "opcodelist")){
            list_push(inst_input);
            opcodelist();
        }
        else if(!strcmp(inst_input, "quit") || !strcmp(inst_input, "q")){  // quit
            quit();
        }
        else if(!strcmp(inst_input, "reset")){
            list_push(inst_input);
            reset();
        }
        else if(!strcmp(inst_input, "symbol")){
            list_push(inst_input);
            symbol();
        }
    }
    else if (blank == 1){ //dump start처럼 띄어쓰기가 1개!
        char blank1[MAX_STR];
        int i = 0;
        argument_extractor(inst_input, blank1, &i); // 띄어쓰기가 있으면 Argument가 있당
        // 걔를 추출해야 써먹지 우선 이 명령어만 추출하자. 
        // 여기있는 argument_extractor 명령어를 추출한다. 

        if (!strcmp(blank1, "dump") || !strcmp(blank1, "du"))
        { 
            list_push(inst_input);
            int start_point = last_number_extractor(inst_input, &i);
            // 첫번쨰 매개변수 추출하기~

            if(start_point == ERR){
                printf("DUMP 'START' ERROR\n");
                return;
            }
            dump(start_point, -1, 1);
        }

        else if(!strcmp(blank1, "opcode")){
            list_push(inst_input);
            int j = 0;
            char inkey[MAX_STR];

            while(inst_input[i] == ' ')
                    i++;
            while(j < MAX_STR && i < MAX_STR && inst_input[i] != ' '){
                inkey[j++] = inst_input[i++];
            }
            // 이 함수는 매개변수가 문자열이다.

            inkey[j] = '\0';
            opcode_mnemonic(inkey);
            return;
        }
        else if(!strcmp(blank1, "assemble")){
            list_push(inst_input);
            int j = 0;
            char inkey[MAX_STR];

            while(inst_input[i] == ' ')
                    i++;
            while(j < MAX_STR && i < MAX_STR && inst_input[i] != ' '){
                inkey[j++] = inst_input[i++];
            }
            assemble(inkey);
        }
        else if(!strcmp(blank1, "type")){
            list_push(inst_input);
            int j = 0;
            char inkey[MAX_STR];

            while(inst_input[i] == ' ')
                    i++;
            while(j < MAX_STR && i < MAX_STR && inst_input[i] != ' '){
                inkey[j++] = inst_input[i++];
            }
            type(inkey);
        }
    }
    else if(blank == 2){ // edit address, value 처럼 빈 칸이 두개! 
        char blank1[MAX_STR];
        int i = 0;
        argument_extractor(inst_input, blank1, &i); // 명령어 부터 추출해보자. 

        if (!strcmp(blank1, "edit") || !strcmp(blank1, "e"))
        {
            int address, value;

            address = first_number_extractor(inst_input, &i); // 첫번째 매개변수 추출 
            if(address == ERR){
                printf("EDIT 'ADDRESS' ERROR\n");
                return;
            }

            if(rest_and_blank(inst_input, &i) == ERR){   // 공백 제거 
                printf("EDIT ERROR ADDRESS <---> VALUE\n");
                return;
            }
            
            value = last_number_extractor(inst_input, &i);  // 두 번째 매개변수 추출 
            if(value == ERR){
                printf("EDIT ERROR VALUE \n");
                return;
            }
            list_push(inst_input);
            edit(address, value);   //함수 실행 
        }

        else if (!strcmp(blank1, "dump") || !strcmp(blank1, "du"))
        {      
            int b1 = 0, b2 = 0;
            b1 = first_number_extractor(inst_input, &i);    // 첫번째 매개변수 

            if(b1 == ERR){
                printf("DUMP FIRST ARGUMNET ERROR\n");
                return;
            }

            if (rest_and_blank(inst_input, &i) == ERR){ // 공백 제거 
                printf("DUMP ERROR BETWEEN FIRST <----> SECOND ARGU\n");
                return;
            }
            b2 = last_number_extractor(inst_input, &i); //두번째 매개변수
            if(b2 == ERR){
                printf("DUMP ERROR AT LAST ARGUMENT\n");
                return;
            }
            list_push(inst_input);
            dump(b1, b2, 2);    // b1 ~ b2 인쇄하기!
        }
    }

    else if(blank == 3){ // fill start, end, value 처럼 빈칸이 3개!
        char blank1[MAX_STR];
        int i = 0;
        argument_extractor(inst_input, blank1, &i); // 명령어는 무엇이냐

        if(!strcmp(blank1, "fill") || !strcmp(blank1, "f")){
            int start, end, value;
            start = first_number_extractor(inst_input, &i);  // 첫번쨰 매개변수!
            if(start == ERR){
                printf("FILL ERROR AT START ARGU\n");
                return;
            }

            if(rest_and_blank(inst_input, &i) == ERR){  // 매개변수 사이의 빈칸 제거 
                printf("FILL ERROR START <---> END\n");
                return;
            }

            end = middle_number_extractor(inst_input, &i); // 두 번쨰 매개변수
            if(end == ERR){
                printf("FILL ERROR AT END ARGU\n");
                return;
            }
            
            if(rest_and_blank(inst_input, &i) == ERR){ // 두 번째와 세번째 매개변수 사이 공백
                printf("FILL ERROR END <---> VALUE\n");
                return;
            }

            value = last_number_extractor(inst_input, &i); // 세 번째 매개변수 무엇이냐
            if(value == ERR){
                printf("FILL ERROR AT VALUE ARGU\n");
                return;
            }
            list_push(inst_input);
            fill(start, end, value); // fill 함수~
        }
            return;
    }
}


/* 매개변수가 정상적인지 체크한다. */
bool is_range(char a){
    if(!((a >= '0' && a <= '9') ||(a >= 'a' && a <= 'f')|| (a >= 'A' && a <= 'F'))){
        return false;
    }
    return true;
}

/* 첫번째 매개변수 추출하기 */
int first_number_extractor(char inst_input[MAX_STR], int *i){
    int first = 0;
    while (inst_input[*i] != ' ' && inst_input[*i] != ',')
    {
        char a = inst_input[(*i)++];
        if (!is_range(a))
        {
            return ERR;
        }
        first *= 16;
        if(a >= '0' && a <='9'){
            first += (a - '0');
        }
        else if(a >= 'a' && a <= 'f'){
            first+= (a - 'a') + 10;
        }
        else if(a >= 'A' && a <= 'F'){
            first += (a - 'A') + 10;
        }
    }
    return first;
}

/*두 번째 매개변수 추출하기 */ 
int middle_number_extractor(char inst_input[MAX_STR], int *i){
    int middle = 0;
    while (inst_input[*i] != ' ' && inst_input[*i] != ',')
    {
        char a = inst_input[(*i)++];
        if(!is_range(a)){
            return ERR;
        }
        middle *= 16;
        if(a >= '0' && a <='9'){
            middle += (a - '0');
        }
        else if(a >= 'a' && a <= 'f'){
            middle += (a - 'a') + 10;
        }
        else if(a >= 'A' && a <= 'F'){
            middle += (a - 'A') + 10;
        }
    }
    return middle;
}

/* 마지막 매개변수 추출하기 */
int last_number_extractor(char inst_input[MAX_STR], int *i){
    int last = 0;
    while (inst_input[*i] != '\0')
    {
        char a = inst_input[(*i)++];
        if(!is_range(a)){
            return ERR;
        }
        last *= 16; // 16진수라서 곱해줍니당
        if(a >= '0' && a <='9') 
            last += (a - '0');
        else if(a >= 'a' && a <= 'f')
            last += (a - 'a') + 10;
        else if(a >= 'A' && a <= 'F')
            last += (a - 'A') + 10;
        
    }
    return last;
}

/* 매개변수 사이에 있는 빈칸 제거하기 */
int rest_and_blank(char inst_input[MAX_STR], int* i){
    if(inst_input[(*i)++] != ','){
        return ERR;
    }

    while (inst_input[++(*i)] == ' ');
    return 1;
}

/* 명령어 뽑아내기 
inst_input에서 뽑아내서 des에 넣을 것이다. 
*/
void argument_extractor(char inst_input[MAX_STR], char des[MAX_STR], int *i){
    while (inst_input[*i] != ' ')
        des[*i] = inst_input[(*i)++];

    des[(*i)++] = '\0';
}
