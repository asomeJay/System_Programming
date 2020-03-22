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
#include "Shell.h"

#define ERR -987654321

void operation(char inst_input[20], int blank);
bool is_range(char a);
int first_number_extractor(char inst_input[20], int *i);
int middle_number_extractor(char inst_input[20], int* i);
int last_number_extractor(char inst_input[20], int *i);
void rest_and_blank(char inst_input[20], int *i);

int main(void)
{
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
        /* Argument Extractor */
        char blank1[20];
        int i = 0;

        while (inst_input[i] != ' ')
            blank1[i] = inst_input[i++];

        blank1[i++] = '\0';
        /* Argument Extractor */

        if (!strcmp(blank1, "edit") || !strcmp(blank1, "e"))
        {
            int b1 = 0, b2 = 0;
            printf("EDIT START\n");
            while (inst_input[i] != ' ' && inst_input[i] != ',')
            {
                char a = inst_input[i++];
                if (!((a > '0' && a < '9') || (a >= 'A' && a <= 'F')))
                {
                    printf("EDIT 'ADDRESS' ERROR\n");
                    return;
                }

                b1 *= 16;
                if(a > '0' && a <'9'){
                    b1 += (a - '0');
                }
                else if(a >= 'A' && a <= 'F'){
                    b1 += (a - 'A') + 10;
                }
            }
            if(inst_input[i++] != ','){
                printf("EDIT ERROR\n");
                return;
            }

            while (inst_input[++i] == ' ')
                ;

            while (inst_input[i] != '\0')
            {
                char a = inst_input[i++];
                if(!((a > '0' && a < '9') || (a >= 'A' && a <= 'F'))){
                    printf("EDIT 'VALUE' ERROR\n");
                    return;
                }
                b2 *= 16;
                if(a > '0' && a <'9'){
                    b2 += (a - '0');
                }
                else if(a >= 'A' && a <= 'F'){
                    b2 += (a - 'A') + 10;
                }
            }
            printf("%d %d\n", b1, b2);
            list_push(inst_input);
            edit(b1, b2);
            printf("EDIT END\n");
        }

        else if (!strcmp(blank1, "dump") || !strcmp(blank1, "du"))
        {      
            int b1 = 0, b2 = 0;
            b1 = first_number_extractor(inst_input, &i);

            if(b1 == ERR){
                printf("DUMP FIRST ARGUMNET ERROR\n");
                return;
            }

            rest_and_blank(inst_input, &i);

            if (i == ERR){
                printf("DUMP ERROR BETWEEN FIRST <----> SECOND ARGU\n");
                return;
            }
                /*while (inst_input[i] != ' ' && inst_input[i] != ',')
            {
                char a = inst_input[i];
                if(!is_range(a)){
                    printf("DUMP ERROR\n");
                    return;
                }
                b1 *= 16;
                b1 += inst_input[i++] - '0';
            }
                if (inst_input[i++] != ',')
                {
                    printf("DUMP ERROR\n");
                    return;
                }*/

            /*while (inst_input[++i] == ' ')
                ;*/
            b2 = last_number_extractor(inst_input, &i);
            if(b2 == ERR){
                printf("DUMP ERROR AT LAST ARGUMENT\n");
                return;
            }
            /*while (inst_input[i] != '\0')
            {
                char a = inst_input[i];
                if(!is_range(a)){
                    printf("DUMP ERROR\n");
                    return;
                }
                b2 *= 16;
                b2 += inst_input[i++] - '0';
            }*/

            list_push(inst_input);
            dump(b1, b2, 2);
        }
    }

    else if(blank == 3){
        char blank1[20];
        int i = 0;

        while (inst_input[i] != ' ')
            blank1[i] = inst_input[i++];

        blank1[i++] = '\0';

        if(!strcmp(blank1, "fill") || !strcmp(blank1, "f")){
            int start, end, value;
            start = first_number_extractor(inst_input, i);
            if(start == ERR){
                printf("FILL ERROR AT START ARGU\n");
                return;
            }

            rest_and_blank(inst_input, i);
            if(i == ERR){
                printf("FILL ERROR START <---> END\n");
                return;
            }

            end = middle_number_extractor(inst_input, i);
            if(end == ERR){
                printf("FILL ERROR AT END ARGU\n");
                return;
            }

            rest_and_blank(inst_input, i);
            if(i == ERR){
                printf("FILL ERROR END <---> VALUE\n");
                return;
            }

            value = last_number_extractor(inst_input, i);
            if(value == ERR){
                printf("FILL ERROR AT VALUE ARGU\n");
                return;
            }
            list_push(inst_input);
            fill(start, end, value);
        }
            return;
    }
}

bool is_range(char a){
    if(!((a > '0' && a < '9') || (a >= 'A' && a <= 'F'))){
        return false;
    }
    return true;
}

int first_number_extractor(char inst_input[20], int *i){
    int first = 0;
    while (inst_input[*i] != ' ' && inst_input[*i] != ',')
    {
        char a = inst_input[(*i)++];
        if (!is_range(a))
        {
            return ERR;
        }
        first *= 16;
        first += (a - '0');
    }
    return first;
}

int middle_number_extractor(char inst_input[20], int *i){
    int middle = 0;
    while (inst_input[*i] != ' ' && inst_input[*i] != ',')
    {
        char a = inst_input[(*i)++];
        if(!is_range(a)){
            return ERR;
        }
        middle *= 16;
        middle += (a - '0');
    }
    return middle;
}

int last_number_extractor(char inst_input[20], int *i){
    int last = 0;
    while (inst_input[*i] != '\0')
    {
        char a = inst_input[(*i)++];
        if(!is_range(a)){
            return;
        }
        last *= 16;
        last += (a - '0');
    }
    return last;
}

void rest_and_blank(char inst_input[20], int* i){
    if(inst_input[(*i)++] != ','){
        return ERR;
    }

    while (inst_input[++(*i)] == ' ');
}
