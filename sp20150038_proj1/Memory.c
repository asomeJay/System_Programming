#ifndef __Memory_C__
#define __Memory_C__

#include "Memory.h"
#include <stdio.h>

int address = 0;

void dump(int dump_start, int dump_end){
    int i, j;
    for (i = 0; i < 10; i++, address += 16){
        printf("%05X", address);
        printf(" ");
        for (j = 0; j < 16; j++)
        {
            printf("%02X ", memory[i * 16 + j]);
        }
        printf(" ; ");
        for(j = 0; j < 16; j++){
            if(memory[i * 16 + j] < 0x20 || 
            memory[i * 16 + j] > 0x7E){
                printf(".");
            }
            else {
                printf("%c", memory[i*16 + j]);
            }
        }

        printf("\n");
    }
}

#endif
