#ifndef __DEBUG_H__
#define __DEBUG_H__

void bp_set(int address); // break Point
void bp_clear(); // sicsim에 존재하는 breakpoint를 전부 삭제한다.
void bp(); // sicsim에 존재하는 breakpoint를 전부 화면에 출력한다.

#endif // __LINKING_H__
