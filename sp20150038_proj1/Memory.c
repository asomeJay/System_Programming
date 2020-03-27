#include "Memory.h"
#include <stdio.h>

int address = 0;

void dump(int dump_start, int dump_end, int flag){
    int i, j;

    if(flag == 2){
        if(dump_start > dump_end){
            printf("FUNCTION 'DUMP' RANGE ERROR\n");
            return;
        }
            
        if(dump_start < 0 || dump_start > 0xFFFFF || 
        dump_end <0 || dump_end > 0xFFFFF){
            printf("FUNCTION 'DUMP' RANGE ERROR\n");
            return;
        }
        dump2(dump_start, dump_end);
        return;
    }

    if (flag == 0) {
            dump_start = address;
            dump_end = address + 10;
    }

    else if(flag == 1){
        if(dump_start <0 || dump_start > 0xFFFFF){
            printf("FUNCTION 'DUMP' RANGE ERROR\n");
            return;
        }

        dump_end = dump_start + 10;
    }
    
    for (i = 0 ; i < 10; i++){
        printf("%04X0 ", dump_start / 16);

        for (j = 0; j < 16; j++) {
            if(dump_start + i * 16 + j < dump_start ) {
                printf("   ");
                continue;
            }
            printf("%02X ", memory[i * 16 + j]);
        }
        printf(" ; ");

        for(j = 0; j < 16; j++){
            if(memory[dump_start + i * 16 + j] < 0x20 || 
            memory[dump_start + i * 16 + j] > 0x7E){
                printf(".");
            }
            else {
                printf("%c", memory[i*16 + j]);
            }
        }
        printf("\n");

        dump_start += 16;

        if (dump_start > 0xFFFFF){
            dump_start = 0;
        }
    }

    if(flag == 0)
        address = dump_start;
}

void dump2(int dump_start, int dump_end){
    int i, j, s_f, e_f;
    s_f = 0;
    e_f = 0;
    while (dump_start <= dump_end)
    {        
        printf("%04X0 ", (int)(dump_start / 16));
        if (s_f == 0)
        {
            for (i = 0; i < dump_start % 16; i++)
                printf("   ");
            s_f = 1;
        }
        for (i = dump_start % 16; i < 16 && dump_start <= dump_end; i++){
            printf("%02X ", memory[dump_start++]);
        }
        if(dump_start > dump_end){
            while (dump_start % 16 != 0)
            {
                printf("   ");
                dump_start++;
            }
        }
        printf(" ; ");
        dump_start -= 16;
        for (j = 0; j < 16; j++)
        {
            if(dump_start > dump_end){
                printf(".");
                continue;
            }
            if(memory[dump_start] < 0x20 || memory[dump_start] > 0x7E)
            {
                printf(".");
            }
            else 
            {
                printf("%c", memory[dump_start]);
            }
            dump_start++;
        }
        printf("\n");
    }
}

void edit(int address, int value){
    memory[address] = value;
}

void fill(int start, int end, int value){
    int i;
    for (i = start; i <= end; i++)
    {
        memory[i] = value;
    }
}

void reset(){
    int i;
    for (i = 0; i < (1 << 20); i++)
    {
        memory[i] = 0;
    }
}
