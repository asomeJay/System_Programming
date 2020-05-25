#include "Linking_Loader.h"
#include "Memory.h"
#include "Debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run()
{
    int current_inst, current_format; current_record_address = -1;
    int i;
    if (current_record_address == -1) //At the firsttime
        current_record_address = execute_address;
    registers[PC] = execute_address;
    
    while(current_record_address < end_address){
        current_inst = memory[current_record_address] % 0XFC;
        // decode opcode (instruction)
        // nixbpe
        int format_standard = memory[current_record_address] / 0XF;
        printf("format_standara : %d \n", format_standard);
        switch (format_standard)  {
        case 0xE:
            if (memory[current_record_address + 1] & 0XF)  {
                // e bit is set
                current_format = 4;
            }
            else  {
                current_format = 3;
            }
            break;
        case 0xB:
            current_format = 2; // format 2
        }

        /* format1 = 1byte format2 = 2byte, format3 = 3byte, format4 = 4byte*/
        for (i = current_record_address; i < current_record_address + current_format; i++){
            if(bp_search(i) == 1) {
                printf("A : %06X  X : %06X\n", registers[A], registers[X]);
                printf("L : %06X  PC : %06X\n", registers[L], registers[PC]);
                printf("B : %06X  S : %06X\n", registers[B], registers[S]);
            }
        }
        // memory empty? -> continue(PC update)
        if(!memory[current_record_address]){
            registers[PC]++;
            current_record_address = registers[PC];
            continue;
        }

        // update PC Register
        registers[PC] += current_format;
        current_record_address = registers[PC];
    }
    printf("A : %06X  X : %06X\n", registers[A], registers[X]);
    printf("L : %06X  PC : %06X\n", registers[L], registers[PC]);
    printf("B : %06X  S : %06X\n", registers[B], registers[S]);
}

void loader(FILE **fp_list){
    int i;
    // Symbol Table Initializing
    for (i = 0; i < 200; i++) {
        symbol_table[i].symbol = NULL;
        symbol_table[i].addr = 0;
        ext_table[i].symbol = NULL;
        ext_table[i].addr = 0;
    }
    // Initi FInish

    for (i = 0; i < REG_COUNT; i++)
    {
        registers[i] = 0; // initializing registers
    }

    int program_length = link_loader_pass1(fp_list); // pass1 실행
    if(program_length == ERROR)
        return;
    int execute_address = link_loader_pass2(fp_list); // pass2 실행
    if(execute_address == ERROR)
        return;

    symbol_index = 0;
    for (i = 0; i < symbol_index; i++){
        printf("%s : %d\n", symbol_table[i].symbol, symbol_table[i].addr);
    }
        printf("-----------------------------------------\n");
        printf("\t\t\t\t\t Total length\t%04X\n", program_length);

        registers[L] = linking_address + program_length;

        for (i = 0; i < 3; i++)
        {
            if(fp_list[i] != NULL){
                fclose(fp_list[i]);
            }
        }

        return;
}

int link_loader_pass1(FILE ** fp_list){
    int i, j, k, fp_index, control_section_address, es_address;
    char current_record[MAX_RECORD], cs_name[MAX_RECORD], es_name[MAX_RECORD];
    i = 0; j = 0; k = 0; fp_index = 0;
    control_section_address = 0;

    while (fp_list[fp_index] != NULL)
    {
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
        while(current_record[j] != '\n'){
            control_section_address *= 10;
            control_section_address += (current_record[j++] - '0');
        }

        if(is_control_section(cs_name)==CAN_FIND){ // Already Control Section there
            printf("Control Section already be");
            return ERROR; // return ERROR
        }
        symbol_table[symbol_index].addr = control_section_address;
        symbol_table[symbol_index++].symbol = cs_name;

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
                while(current_record[m] != ' '){
                    es_name[n++] = current_record[m++];
                }
                es_name[n] = '\0';

                while(current_record[m] == ' ')
                    m++;

                /* Getting ES ADDRESS*/
                while(current_record[m] != ' ' ){
                    es_address *= 10;
                    es_address += current_record[m] - '0';
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

int link_loader_pass2(FILE **fp_list)  {
    int  j, fp_index = 0, length, offset;
    int CSLTH, control_section_address;
    char current_record[MAX_RECORD], temp[20];

    control_section_address = linking_address; // start address setting

    while (fp_list[fp_index])// while not end of input do
    {                                                         
        fgets(current_record, MAX_RECORD, fp_list[fp_index]); // read next input record
        if(current_record[strlen(current_record) - 1] == '\n')
            current_record[strlen(current_record) - 1] = '\0';
        // HEADER RECORD
        /* set CSLTH to control section length */
        CSLTH = symbol_index;
        fgets(current_record, MAX_RECORD, fp_list[fp_index]); // read next input record
        if(current_record[strlen(current_record) - 1] == '\n')
            current_record[strlen(current_record) - 1] = '\0';

        while(current_record[0] != 'E'){// while record type != E do
            if(current_record[0] == 'T')  {
                // if record type == 'T' then
                // convert into internal representation
                // T Record의 2 ~ 6은 offset을 나타내고 7 ~ 8은 길이를 나타낸다.
                /*
                2 ~ 6 : offset
                7 ~ 8 : length
                */
                memset(temp, '\0', 10);
                strncpy(temp, current_record + 1, 6);

                offset = hex_to_dec(temp);

                memset(temp, '\0', 10);
                strncpy(temp, current_record + 7, 2);

                length = hex_to_dec(temp);
                
                // 그 다음 record의 2 byte씩 묶어서 decimal로 전환한다.
                for (j = 0; j < 10; j++){
                    memset(temp, '\0', 10);
                    strncpy(temp, current_record + 9 + j * 2, 2);
                    // 전환된 값을 메모리에 올린다.    
                    memory[control_section_address + offset + j] = hex_to_dec(temp);
                }

            }
            else if(current_record[0] == 'M'){ // modification Record
            // record type == 'M' 
                /* 
                2 - 7 : offset
                8 - 9 : length 
                etc : reference Number
                */
                memset(temp, '\0', 10);
                strncpy(temp, current_record + 1, 6);

                offset = hex_to_dec(temp);

                memset(temp, '\0', 10);
                strncpy(temp, current_record + 7, 2);
                length = hex_to_dec(temp);

                // search ESTAB for modifying symbol name
                // 그 전에 reference Number부터 얻어내자 이번 플젝은 두자리안임
                memset(temp, '\0', 10);
                strncpy(temp, current_record + 10, 2);
                int reference_number = hex_to_dec(temp);

                // reference_number는 널뛰기 안함.
                if(reference_number > symbol_index){
                    // set error flag. (undefined external symbol)
                    printf("ERROR AT LOADER PASS2 : No reference Num\n");
                    return ERROR;
                }

                
                // reference_number에 해당하는 ES 존재~
                // add or subtract symbol value at location

                
            }
            // if address is specified(in End record) set EXECADDER to
            // CSADDR + specified address

            /* add CSLTH to CSADDR*/
            fgets(current_record, MAX_RECORD, fp_list[fp_index]);
            if(current_record[strlen(current_record) - 1] == '\n')
                current_record[strlen(current_record) - 1] = '\0';

            control_section_address += CSLTH;

        }

        fp_index++;
        if(fp_index > 2)
            break;
    }
    return EXECADDR;
}

int is_control_section(char * cs_name)  {
    /* Symbol Table에서 그게 있는지 찾는다. */
    int i;

    for(i = 0; i < symbol_index; i++){
        if(!strcmp(cs_name, symbol_table[i].symbol)){
            return CAN_FIND;
        }
    }
    return CANT_FIND;
}

int is_estab(char * a){
    int i;
    for (i = 0; i < ext_index; i++){
        if(!strcmp(a, ext_table[i].symbol)){
            return CAN_FIND;
        }
    }
    return CANT_FIND;
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

int hex_to_dec(char * hex)  {
    int i, dec = 0;
    for (i = 0; i < strlen(hex); i++)  {
        dec *= 16;
        dec += (hex[i] - '0');
    }
    return dec;
}

