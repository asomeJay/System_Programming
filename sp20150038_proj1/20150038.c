/**********************************
 * JaeWon KIM's Project 1       ***
 * Plz Don't Copy me            ***
 * I'm Junior of Junior         ***
 * *******************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Basic.h"
#include "Memory.h"
#include "Opcode.h"
#include "Shell.h"

void operation(char inst_input[20], int blank);

int main(void){
    init();

    while (1)
    {
        int i, blank;
        char inst_input[20];

        printf("sicsim> "); // 계속 출력하는 것
        blank = in(inst_input);    // inst_input으로 명령 받고 명령어에 빈칸 몇 갠지 return 받는다.
        operation(inst_input, blank);
    }

    return 0;
}

/* 명령어 들어온 걸로 실제 그 함수를 실행한다. 
실행할 때마다 history list에 저장한다.*/

void operation(char inst_input[20], int blank){
    if(blank == 0){
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
        }
        else if(!strcmp(inst_input, "quit") || !strcmp(inst_input, "q")){  // quit
            quit();
        }
        else if(!strcmp(inst_input, "reset")){
            list_push(inst_input);
        }
    }
    else if (blank == 1){
        char blank1[20];
        int i = 0;
        while (inst_input[i] != ' '){
            blank1[i] = inst_input[i++];
        }
        blank1[i] = '\0';
        i++;

        if (!strcmp(blank1, "dump") || !strcmp(blank1, "du"))
        { 
            list_push(inst_input);

            char blank2[20];
            int j = 0;
            while (inst_input[i] != '\0')
            {   if(inst_input[i] < '0' || inst_input[i] > '9'){
                    printf("DUMP ERROR\n");
                    return;
                }
                blank2[j++] = inst_input[i++];
            }
            blank2[j] = '\0';
            dump(atoi(blank2), -1, 1);
        }

        else if(!strcmp(inst_input, "opcode mnemonic")){
            list_push(inst_input);
            //opcode_mnemonic();
        }

        return;
    }
    else if(blank == 2){
        char blank1[20];
        int i = 0;
        while (inst_input[i] != ' ')
        {
            blank1[i] = inst_input[i++];
        }
        i++;

        if(!strcmp(inst_input, "edit") || !strcmp(inst_input, "e"))
            return;     
        else if (!strcmp(blank1, "dump") || !strcmp(blank1, "du"))
        {
            char blank2[20], blank3[20];
            while (inst_input[i] != ' ')
            {   if(inst_input[i] < '0' || inst_input[i] >'9'){
                    printf("DUMP ERROR\n");
                    return;
                }
                blank2[i] = inst_input[i++];
            }
            while(inst_input[i] != '\0'){
                if(inst_input[i] < '0' || inst_input[i] >'9'){
                    printf("DUMP ERROR\n");
                    return;
                }
                blank3[i] = inst_input[i++];
            }
            blank3[i] = '\0';

            list_push(inst_input);
            dump(atoi(blank2), atoi(blank3), 2);
        }


    }

    else if(blank == 3){
        list_push(inst_input);
        if(!strcmp(inst_input, "fill") || !strcmp(inst_input, "f"))
            return;
    }
}
