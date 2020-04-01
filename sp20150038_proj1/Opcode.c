#include "Opcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void opcode_make(){
    hash_init();
    const char *mode = "r";
    FILE *fp = fopen("opcode.txt", "r");
    //opcode.txt를 읽기 모드로 엽니다. 
    if (fp == NULL)
    {
        printf("FILE READ ERROR\n");
        return;
    }
    // 파일 끝까지 읽어가면서 OPCODE 정보를 value와 temp에 저장합니다.
    while(!feof(fp)){
        int i, value = 0;
        char *str, *sp;
        char buffer[255], temp[255];
        for (i = 0; i < 255; i++){
            buffer[i] = temp[i] = 0;
        }
        str = fgets(buffer, sizeof(buffer), fp);
        for (i = 0; i < 2; i++)
        {
            if (buffer[i] >= '0' && buffer[i] <= '9')
            {
                value *= 16;
                value += (buffer[i] - '0');
            }
            else if (buffer[i] >= 'A' && buffer[i] <= 'Z')
            {
                value *= 16;
                value += (buffer[i] - 'A' + 10);
            }
            else
            {
                value *= 16;
                value += (buffer[i] - 'a' + 10);
            }
        }

        while (!(buffer[i] >= 'A' && buffer[i] <= 'Z'))
            i++;

        int j = 0;
        while (buffer[i] >= 'A' && buffer[i] <= 'Z')
        {
            temp[j++] = buffer[i++];
        }
        temp[j] = '\0';
        hash_add(temp, value); // 읽은 값을 바탕으로 해쉬에 추가합니다. 
    }

    fclose(fp); // 역할을 다했으니 파일을 닫습니다. 
}

/* 해쉬 테이블을 초기화합니다. */
void hash_init(){
    int i;
    for (i = 0; i < MAX_HASH; i++){
        hash_table[i] = NULL;
    }
}

/*
명령어와 그 명령어의 value를 매개변수로 받는다.
명령어를 통해 해쉬 테이블에서 어디에 위치할 지 찾는다.(hash_function 사용)
만약 그 위치에 다른 값이 이미 들어있다면 연결 리스트 특성을 이용해서
그 노드의 Next에 연결한다.
만약 그 위치에 다른 값이 없다면, 이 노드가 최초로 그곳에 있게되므로 그냥 assign 한다
*/

int hash_add(const char *inkey, int value){
    /* 새롭게 노드를 만들고 매개변수를 그 노드에 삽입하는 과정 */
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node == NULL){
        printf("HASH_ADD MALLOC ERROR\n");
        return -1;
    }
    strcpy(new_node->key, inkey);
    new_node->value = value;
    new_node->next = NULL;

    /* 매개변수 inkey를 이용해서 해쉬테이블에 어디에 위치할 지 보자*/
    int hash_index = hash_function(inkey);

    /* 그 위치에 다른 애가 없다? 아싸 바로 넣어야지 */
    if(hash_table[hash_index] == NULL){
        hash_table[hash_index] = new_node;
    }
    /* 그 위치에 다른 애가 있네... 줄 제일 끝으로 가야겠다*/
    else{
        Node *current_node = hash_table[hash_index];
        while(current_node->next != NULL)
            current_node = current_node->next;
        current_node->next = new_node;
    }
    return 1;
}

/*
inkey에 해당하는 value를 찾을 것이다. 늘 그랬듯이 */
int hash_find(const char *inkey){
    // 매개변수 inKey를 이용해서 hash_table에서 이 string이 
    // 어느 Bucket에 있는지 찾는다. 
    int hash_index = hash_function(inkey);

    //그 찾은 위치의 제일 앞에 있는 node를 cur이라고 명명한다. 
    Node *cur = hash_table[hash_index];

    // 그 노드부터 차례로 쭉 가면서 내가 찾으려는 Instruction이랑 일치하는 애가 있는가?
    while(strcmp(cur->key, inkey) != 0){
        cur = cur->next;

        if(cur == NULL){
            // 일치하는 애가 없으면...  
            // 없으면...!! -1을 return 하자. 이건 일종의 에러인데, 에러처리는 다른 함수에서
            // 해줄 것이다.
            return -1;
        }
    }
    // 일치하는 애를 return 해준다. 
    return cur->value;
}


int hash_function(const char *str){
    int hash = 401;
    int i;

    while (*str != '\0'){
        hash = ((hash << 4) + (int)(*str)) % MAX_HASH;
        str++;
    }

    return hash % MAX_HASH;
}

/* 명령어를 입력하면 해당하는 opcode를 출력하는 함수
매개변수로 명령어를 받고, hash_find 함수로 그 opcode를 출력하면 된다. 
*/
void opcode_mnemonic(const char *inkey){
    // 관상가 양반, 내가 opcode가 될 상인가!
    int opcode_candidate = hash_find(inkey);
    // 아니옵니다... 에러 상이옵니다.
    if(opcode_candidate == -1){
        printf("'%s' Not in Hash\n", inkey);
        return;
    }

    // OPCODE가 될 상이옵니다. 
    printf("opcode is %X\n", opcode_candidate);
}

/* 프로그램 시작할 떄 opcode list를 만드는데 그걸 잘 읽도록 합니다 */ 
void opcodelist(){
    int i;
    for (i = 0; i < 20; i++){
        Node *cur = hash_table[i];
        printf("%d : ", i);
        if(cur != NULL){
            printf("[%s,%d]", cur->key, cur->value);
            cur = cur->next;
        }
        while (cur != NULL)
        {
            printf(" -> [%s,%d]", cur->key, cur->value);
            cur = cur->next;
        }
        printf("\n");
    }
}

