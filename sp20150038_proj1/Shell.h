#ifndef __Shell_h__
#define __Shell_h__

void help(){
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("%s\n", inst_list[i]);
    }
    printf('\n');
}

#endif