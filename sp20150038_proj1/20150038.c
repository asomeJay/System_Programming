/**********************************
 * JaeWon KIM's Project 1       ***
 * Plz Don't Copy me            ***
 * I'm Junior of Junior         ***
 * *******************************/

#include "./20150038.h"
#include "./Memory.h"
#include "./Opcode.h"
#include "./Shell.h"

int main(void){
    init();

    while (1)
    {
        int i, inst_index = 0, blank = 0;
        char inst_input[20] , c;
        char *extracted_instruction;

        printf("sicsim> ");
        while((c = getchar()) != '\n'){
            if(c == ' ')
                blank++;
            inst_input[inst_index++] = c;
        }
        inst_input[inst_index] = '\0';
        
        extracted_instruction = instruction(inst_input, blank);
        if(strcmp(extracted_instruction, "NULL") == 0)
            continue;
        
    }
    return 0;
}

char * instruction(char inst_input[20], int blank){
    int i;
    
    if(blank == 0){
        if(!strcmp(inst_input, "dir") || !strcmp(inst_input, "d")){
            list_push(inst_input);
            dir();
        }
        if (!strcmp(inst_input, "dump") || !strcmp(inst_input, "du")){
            list_push(inst_input);

        }
        if(!strcmp(inst_input, "help") || !strcmp(inst_input, "h")){
            list_push(inst_input);
            help();
        }
        if(!strcmp(inst_input, "history") || !strcmp(inst_input, "hi")) // history
            list_push(inst_input);
            history();
        if(!strcmp(inst_input, "opcodelist")){
            list_push(inst_input);
        }
        else if(!strcmp(inst_input, "quit") || !strcmp(inst_input, "q"))  // quit
            quit();
        else if(!strcmp(inst_input, "reset")){
            list_push(inst_input);
        }
    }
    else if (blank == 1){
        list_push(inst_input);
        return "opcode menmonic";
    }
    else if(blank == 2){
        if(!strcmp(inst_input, "edit") || !strcmp(inst_input, "e"))
            return "edit";
    }

    else if(blank == 3){
        list_push(inst_input);
        if(!strcmp(inst_input, "fill") || !strcmp(inst_input, "f"))
            return "fill";
    }    
}
