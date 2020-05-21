#include "Memory.h"
#include <stdio.h>

void dump(int dump_start, int dump_end, int flag){
    int i, j;

    if(flag == 2){
        // 주소값 잘못됐습니다.
        if(dump_start > dump_end){
            printf("FUNCTION 'DUMP' RANGE ERROR\n");
            return;
        }
            // 주소값 잘못됐다구요.
        if(dump_start < 0 || dump_start > 0xFFFFF || 
        dump_end <0 || dump_end > 0xFFFFF){
            printf("FUNCTION 'DUMP' RANGE ERROR\n");
            return;
        }

        //주솟값 잘못된 거 없으니 시작!
        dump2(dump_start, dump_end);
        return;
    }

    if (flag == 0) {
        // 기본 모드는 10줄을 출력하는 것이죠~
            dump_start = address;
            dump_end = address + 10;
    }

    else if(flag == 1){ // start 지점만 요청들어왔을 경우 어찌 할까요~
        // 주소가 이상해요~ 
        if(dump_start <0 || dump_start > 0xFFFFF){
            printf("FUNCTION 'DUMP' RANGE ERROR\n");
            return;
        }

        dump_end = dump_start + 10; 
    }
    
    /* 16진수 형식에 맞춰서 출력합니다 */
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

        if (dump_start > 0xFFFFF){ // 만약 메모리가 끝까지 갔다면 0으로 초기화해주죠
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
    /* start와 end를 지정해주는 경우에 대해 인쇄하기 */ 
    //16진수 형식에 맞춰서...
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
    // 매개변수로 들어온 주소에 value 넣기~ 
    memory[address] = value;
}

/* 매개변수 start, end 영역에 value 넣어주기 */
void fill(int start, int end, int value){
    int i;
    for (i = start; i <= end; i++)
    {
        memory[i] = value;
    }
}

/* 전부 0으로 만들어버릴겁니다 ! */
void reset(){
    int i;
    for (i = 0; i < (1 << 20); i++)
    {
        memory[i] = 0;
    }
}

/* program address를 수정합니다 */ 
void progaddr(int addr){
    address = addr;
    return;
}
