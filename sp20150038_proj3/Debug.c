#include "Debug.h"
#include <stdio.h>

int break_index;

// break Point
void bp_set(int addr){
    break_point[break_index++] = addr;
    return;
}

// sicsim에 존재하는 breakpoint를 전부 삭제한다.
void bp_clear(){
    break_index = 0;
    return;
}

// sicsim에 존재하는 breakpoint를 전부 화면에 출력한다.
void bp(){
    int i;
    printf("\n\tbreakpoint\t\t\t\n");
    printf("\t-----------\n");
    
    for (i = 0; i < break_index; i++)
    {
        printf("\t%d\n", break_point[i]);
    }
    return;
}

int bp_search(int target){
    int i;
    for (i = 0; i < break_index; i++){
        if(target == break_point[i]){
            return 1;
        }
    }
    return 0;
}