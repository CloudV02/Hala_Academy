#include <stdio.h>

#include <stdint.h>

int main(){
    uint8_t number_1 = 125; // 0111 1101
    uint8_t number_2 = 100; // 0110 0100

    // AND
    uint8_t number_and  = number_1 & number_2;
    printf("%d\n", number_and);

    // OR
    uint8_t number_or = number_1 | number_2;
    printf("%d\n", number_or);

    // XOR
    uint8_t number_xor = number_1 ^ number_2;
    printf("%d\n", number_xor);

    // NOT
    uint8_t number_not_1 = ~(number_1);
    uint8_t number_not_2 = ~(number_2);
    printf("%d\n", number_not_1);
    printf("%d\n", number_not_2);

    // SHIFT LEFT 
    uint8_t number_shf_lef = number_1 << 2;
    printf("%d\n", number_shf_lef);

    // SHIFT RIGHT
    uint8_t number_shf_right = number_2 >> 2;
    printf("%d\n", number_shf_right);
}