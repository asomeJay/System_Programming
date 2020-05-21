#include "Linking_Loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int linking_address = 0;

void run(){
    printf("RUN\n");
}

void loader(FILE **fp_list){
    printf("LOADER ENTER\n");
    int i;

    // Symbol Table Initializing
    for (i = 0; i < 200; i++) {
        symbol_table[i].symbol = NULL;
        symbol_table[i].addr = 0;
    }
    // Initi FInish

    for (i = 0; i < REG_COUNT; i++)
    {
        registers[i] = 0; // initializing registers
    }

    link_loader_pass1(fp_list);
    link_loader_pass2(fp_list);
}

int link_loader_pass1(FILE ** fp_list){
    int i, j, k, fp_index, control_section_address, es_address;
    char current_record[MAX_RECORD], cs_name[MAX_RECORD], es_name[MAX_RECORD];
    i = 0; j = 0; k = 0; fp_index = 0;
    control_section_address = 0;

    while (fp_list[fp_index] != NULL)  {
        fgets(current_record, MAX_RECORD, fp_list[fp_index]);
        if (current_record[strlen(current_record) - 1] == '\n')
            current_record[strlen(current_record) - 1] = '\0';

        /* Getting Control Section Name */ 
        j = 1; 
        while (current_record[j] != ' ') {
            cs_name[k++] = current_record[j++];
        } cs_name[k] = '\0';
        
        while (current_record[j] == ' ') 
            j++;
        
        /* Getting Control Section Address */ 
        while(record_check(current_record[j])){
            control_section_address *= 10;
            control_section_address = current_record[j] - '0';
        }
        printf("controlsection : %d\n", control_section_address);

        if(is_control_section(cs_name)){ // Already Control Section there
            printf("Control Section already be");
            return ERROR; // return ERROR
        }

        // If you are here, this control section name is new!

        /* Read Next Record(T) */
        fgets(current_record, MAX_RECORD, fp_list[fp_index]);
        if (current_record[strlen(current_record) - 1] == '\n')
            current_record[strlen(current_record) - 1] = '\0';
        
        while(current_record[0] != 'E'){
            if(current_record[0] == 'D'){
                // for each symbol in the record
                int m = 1, n = 0;
                /* Getting EsNAME */ 
                while(record_check(current_record[m])){
                    es_name[n++] = current_record[m++];
                }
                es_name[n] = '\0';

                while(current_record[m] == ' ')
                    m++;

                /* Getting ES ADDRESS*/
                while(record_check(current_record[m])){
                    es_address *= 10;
                    es_address = current_record[m] - '0';
                }

                // search ESTAB for symbol name
                //if found -> set error flag
                if(!(is_estab(es_name) == CANT_FIND)){
                    printf("ESTAB ERROR\n");
                    return ERROR;
                }

                // else -> enter symbol into ESTAB with value
                symbol_table[symbol_index].symbol = (char *)malloc(sizeof(MAX_CHAR));
                strncpy(symbol_table[symbol_index].symbol, es_name, sizeof(es_name));
                symbol_table[symbol_index].addr = es_address;// add new external          }
            }

            fgets(current_record, MAX_RECORD, fp_list[fp_index]); // read next record
            if(current_record[strlen(current_record) - 1] == '\n')
                current_record[strlen(current_record) - 1] = '\0';
    }

    control_section_address += es_address; // increment CSADDR
    fp_index++;
        /* Load할 파일이 남아있는가 */ 
        if (fp_list[fp_index] != NULL) // add CSLTH to CSADDR
        {
            int m;
            for (m = 0; m < MAX_RECORD; m++)  {
                cs_name[m] = 0;
                es_name[m] = 0;
            }
        }
    }
    end_address = control_section_address;
    return control_section_address - linking_address;
}

int link_loader_pass2(FILE **fp_list){
    return 0;
}

int is_control_section(char * cs_name){
    /* Symbol Table에서 그게 있는지 찾는다. */
    return 0;
}

int is_estab(char * a){
    return 0;
}

int record_check(char target){
    if (target == ' ' || target == '\0' || target == '\n')
        return 0;
    else
        return 1;
}

/* program address를 수정합니다 */ 
void program_addr(int addr){
    linking_address = addr;
    return;
}
