#ifndef __Basic_C__
#define __Basic_C__

#include "Basic.h"

linked_list *start = NULL;
linked_list *last = NULL;


void init() {
    list_make();
}

int in(char inst_input[20]) {
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

void list_make(){
    inst_list = (char **)malloc(sizeof(char *) * 10);

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
}

void dir(){
    DIR *dirp;
    struct dirent *now;

    now = (struct dirent *)malloc(sizeof(struct dirent));

    dirp = opendir(".");

    if(dirp == NULL){
        printf("dirp Error\n");
        exit(-1);
    }

    while((now = readdir(dirp)) != NULL) {
        if(now->d_type == 8){
            printf("%s/\t", now->d_name);
        }

        else if((strstr(now->d_name, ".exe") != NULL) || 
        (strstr(now->d_name, ".out") != NULL)){
            printf("%s*\t", now->d_name);
        }
        else{
            printf("%s\t", now->d_name);
        }
    }
    close(dirp);
    printf("\n");
    return;
}

void quit(){
    exit(0);
}

void history(){
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

void help(){
    int i;
    for (i = 0; i < 10; i++){
        printf("%s\n", inst_list[i]);
    }
    return;
}

void list_push(char inst_[20]) {
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

#endif
