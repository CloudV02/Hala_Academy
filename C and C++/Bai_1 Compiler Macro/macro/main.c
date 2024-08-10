#include <stdio.h>

#define DEFINE_1    0
#define DEFINE_2    1
#define DEFINE_3    2

#define CALL_DEFINE DEFINE_1


#if(CALL_DEFINE == DEFINE_1)
    void test(){
        printf("Define 1");
    }
#elif(CALL_DEFINE == DEFINE_2)
    void test(){
        printf("Define 2");
    }
#else
    void test(){
        printf("Define 3");
    }
#endif
int main()
{
    test();
}