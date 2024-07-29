#include <stdio.h>

extern int a;
extern void cong(int a, int b);

int main(){
    printf("%d\n",a);
    cong(5,6);
}