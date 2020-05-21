#ifndef __Memory_H__
#define __Memory_H__

char memory[1 << 20];
int address = 0; // 주소의 시작은 0입니닷

void dump(int , int, int );
void dump2(int, int);

void edit(int, int);
void fill(int, int, int);
void reset();

void progaddr(int);
#endif
