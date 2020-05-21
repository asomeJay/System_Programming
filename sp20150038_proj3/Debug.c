#include "Debug.h"
#include <stdio.h>

// break Point
void bp_set(int addr){
    printf("bp_set : %d\n", addr);
    return;
}

// sicsim에 존재하는 breakpoint를 전부 삭제한다.
void bp_clear(){
    printf("bp clear\n");
    return;
}

// sicsim에 존재하는 breakpoint를 전부 화면에 출력한다.
void bp(){
    printf("bp \n");
    return;
}
