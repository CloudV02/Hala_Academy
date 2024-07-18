#include <stdio.h>
//#include "test.txt"
#define SIZE 20
#define CREATE_FUNC(name, cmd) \
void name(){                    \
    printf(cmd);                \
}                               

#define CREATE_VAR(name, number) \
int int_##name = number; \
char char_##name; \
double double_##name

#define CREATE_STRING(cmd)  printf(#cmd);

#define TEST(...) __VA_ARGS__

//TEST(5,6, "Hello World");

CREATE_FUNC(test1,"This is func 1\n");
CREATE_FUNC(test2,"This is func 2\n");
CREATE_FUNC(test3,"This is func 3\n");
int main(){
    CREATE_VAR(a, 2);
    CREATE_STRING(abc\n)
    test1();
    test2();
    test3();
    int array[SIZE] = {};
    
}