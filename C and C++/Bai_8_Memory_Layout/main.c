#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void test(){

}

int main()
{
    void *ptr_malloc = NULL;
    void *ptr_calloc = NULL;

    ptr_malloc = (uint16_t*)malloc(2*sizeof(uint8_t));
    ptr_calloc = (uint16_t*)calloc(2,sizeof(uint8_t));

    *(uint8_t*)ptr_malloc = 2314;
    *(uint8_t*)ptr_calloc = 300;

    printf("%d\n", *(uint16_t*)ptr_malloc);
    printf("%d\n", *(uint16_t*)ptr_calloc);

    free(ptr_malloc);
    free(ptr_calloc);
}