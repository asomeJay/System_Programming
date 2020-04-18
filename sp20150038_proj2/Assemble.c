#include <stdio.h>
#include <string.h>
#include "Assemble.h"

int sym_index;

void type(char * filename){
    FILE * fp = fopen(filename, "r");
    if(fp == NULL){
        printf("TYPE ERROR\n");
        return;
    }
    char c;
    while((int)(c = fgetc(fp)) != EOF){
        printf("%c", c);
    }
    
    fclose(fp);
    return;
}

void assemble(char * filename){
    int address = 0;
// 파일을 연다. 문제가 있으면 에러 출력
    FILE * fp = fopen(filename, "r");
    if(fp == NULL){
        printf("assemble error\n");
        return;
    }
// 파일을 쭈우우욱 읽어들인다. 한 줄씩 읽을 건데 넉넉하게 60개 잡아놓자
/* 파일이 끝날 때 까지 읽어들여서 .lst 파일과 .obj 파일을 만든다. */
    while(1){
        char src_line[60];
        char parsed_line[3][60];
        int blank;
        if(fgets(src_line, 60, fp) == NULL){
            // NULL 이면 이제 break 해야지
            break;
        }
        // 소스 파일에서 한 줄 읽었으면 명령어로 바꿔줘야한다.
        blank = parse_line(parsed_line, src_line);
        // 잘 쪼개져서 들어갔을 겁니다.
        // START 면 심볼 생성 안하고 이게 시작점임
        if(strcmp(parsed_line[1], "START") == 0){
          int temp = 0;
          int i;
          for(i = 0; parsed_line[2][i] != '\0'; i++){
              temp *= 16;
              temp += (parsed_line[2][i] - '0');
          }
          address = temp;
          continue;
        }
       // 빈 공간이 두 개면 라벨이 없고 세 개면 라벨 있음
        if(blank == 2){
            // 라벨 없다.
        }
        else if(blank==3){
          // 라벨 있다. -> 심볼임
          push_symbol(address, parsed_line[0]);         
          
        }
        else{
          printf("Assemble Parsing ERROR\n");
          return;
        }
    }
}

void symbol(){
    printf("SYMBOL\n");
   /* 정렬하는 부분 */
   /* 알파벳 순으로 정렬하자... */
    int i, j;
    for(i = 0; i < sym_index; i++){
        for(j = i + 1; j < sym_index; j++){
            if(strcmp(sym_table[i].symbol,sym_table[j].symbol) > 0){
              char temp[8];
              int int_temp;
              strcpy(temp, sym_table[i].symbol);
              strcpy(sym_table[i].symbol, sym_table[j].symbol);
              strcpy(sym_table[j].symbol, temp);
 
              int_temp = sym_table[i].addr;
              sym_table[i].addr = sym_table[j].addr;
              sym_table[j].addr = int_temp;
            }
        }
    }
   

   /* 출력하는 부분 */
    /* 출력은 끝에 다다를 때 까지 하면 그만 */ 
    
    for(i = 0; i < sym_index; i++){
        printf("      ");
        printf("%7s  ", sym_table[i].symbol);
        printf("%04d\n", sym_table[i].addr);
    }
    return;
}

int parse_line(char parse[3][60], char origin[60]){
    int i = 0, j = 0, k = 0, blank = 0;
// 제일 앞이 빈 공간인 거 다 제껴부려~~
    while(origin[i] == ' '){
        i++;
        continue;
    }
    for(; origin[i] != '\0'; i++){
    
        if(origin[i] != ' '){
            parse[j][k] = origin[i];
            k++;
        }
        else {
            parse[j][k] = '\0';
            blank++;
            j++; 
            k = 0;
        }
    }
    return blank;
}

void push_symbol(int addr, char inst[60]){
    // symbol 넣어주기
    strcpy(sym_table[sym_index].symbol, inst);
    // addr 넣기
    sym_table[sym_index].addr = addr;
    sym_index++;
    return;
    
}
