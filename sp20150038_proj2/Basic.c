#include "Basic.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

linked_list *start = NULL;
linked_list *last = NULL;

void init() {
    list_make(); // help 같은 거 할떄 list를 미리 만들어둬야지. 
}

/* 명령어를 받아들이자. c++의 string이 그리워지는 밤입니다. */
int in(char inst_input[MAX_STR]) {
    int blank = 0, inst_index = 0;
    char c;
    while ((c = getchar()) != '\n')
    {
        if (c == ' ')
            blank++;
        inst_input[inst_index++] = c;
    }

    inst_input[inst_index] = '\0';
    return blank;
}

/* help 명령어를 위해 리스트를 만들어줘야지 후후 */
void list_make(){
    inst_list = (char **)malloc(sizeof(char *) * 100);

    inst_list[0] = "h[elp]";
    inst_list[1] = "d[ir]";
    inst_list[2] = "q[uit]";
    inst_list[3] = "hi[story]";
    inst_list[4] = "du[mp] [start, end]";
    inst_list[5] = "e[dit] address, value";
    inst_list[6] = "f[ill] start, end, value";
    inst_list[7] = "reset";
    inst_list[8] = "opcode mnemonic";
    inst_list[9] = "opcodelist";
    inst_list[10] = "assemble filename";
    inst_list[11] = "type filename";
    inst_list[12] = "symbol";
}


void dir(){
    DIR *dirp;
    struct dirent *now;
    struct stat buf;
    now = (struct dirent *)malloc(sizeof(struct dirent));

    dirp = opendir(".");

    if(dirp == NULL){
        printf("dirp Error\n");
        exit(-1);
    }
    while((now = readdir(dirp)) != NULL) {
        lstat(now->d_name, &buf);
        if (S_ISDIR(buf.st_mode)) // DIRECTORY 면 \\ << 얘 붙여줄건데...
            printf("%s/\t", now->d_name); 
            /* 실행파일이면 * 붙여줄거임 ㅎ*/
        else if(strstr(now->d_name, ".exe") != NULL)
            printf("%s*\t", now->d_name);
        else if(strstr(now->d_name, ".out") != NULL)
            printf("%s*\t", now->d_name);
        else
        /*나머지는 그냥~ 그냥~ 그냥!*/
            printf("%s\t", now->d_name);
    }
    closedir(dirp); // 열었으면 닫는건 필수
    printf("\n");
    return;
}

void quit(){ // 시스템 종료
    exit(0);
}

void history(){ // 지금까지 쌓아온 리스트를 그대로 출력하겠습니다. 
    int cnt = 1;
    linked_list *i;
    for (i = start; i != NULL; i = i->next, cnt++)
    {
        if(i->inst == "NULL"){
            continue;
        }
            
        printf("%d\t %s\n", cnt, i->inst);
    }
    return;
}

void help(){ /* inst_list에 저장한 문자열을 출력합니다. */
    int i;
    for (i = 0; i < 13; i++){
        printf("%s\n", inst_list[i]);
    }
    return;
}

void list_push(char inst_[MAX_STR]) { // 명령어가 입력될 때마다 history를 위한 리스트에 넣을거여요! 
    linked_list *t = (linked_list *)malloc(sizeof(linked_list));
    t->next = NULL;
    t->inst = (char *)malloc(sizeof(char) * 20);
    strcpy(t->inst, inst_);

    if(start == NULL){
        start = t;
        last = t;
    }
    else{
        last->next = t;
        last = last->next;
    }
    return;
}

