#ifndef __Memory_H__
#define __Memory_H__

void dump(int start, int end);

void dump(int start, int end){
    int address = 0, i;
    for (i = 0; i < 10; i++, address += 16){
        printf("%05X", address);

    }
        

}
#endif