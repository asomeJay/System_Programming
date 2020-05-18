#ifndef __20150038_H__
#define __20150038_H__

#define ERR -987654321 // 에러넘버는 절대 나오지않을 것 같은 수를 해야 제맛
#define MAX_STR 255

void operation(char inst_input[MAX_STR], int blank);
bool is_range(char a);
int first_number_extractor(char inst_input[MAX_STR], int *i);
int middle_number_extractor(char inst_input[MAX_STR], int* i);
int last_number_extractor(char inst_input[MAX_STR], int *i);
int rest_and_blank(char inst_input[MAX_STR], int *i);
void argument_extractor(char src[MAX_STR], char des[MAX_STR], int *i);

#endif
