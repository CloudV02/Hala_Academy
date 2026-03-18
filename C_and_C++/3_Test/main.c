#include <stdio.h>

int main() {
    char arr[] = {10, 20, 30, 40, 50};
    char *p = &arr[3];   // p trỏ tới arr[3] = 40

    printf("%d\n", p[0]);    // 40
    printf("%d\n", p[-1]);   // 30
    printf("%d\n", p[-2]);   // 20
}
