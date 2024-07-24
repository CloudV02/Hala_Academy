#include <stdio.h>

#define LED1 1 << 0
#define LED2 1 << 1
#define LED3 1 << 2
#define LED4 1 << 3

void enableLED(unsigned int *GPIO_PORT, unsigned int LED){
    *GPIO_PORT |= LED;
}
void disableLED(unsigned int *GPIO_PORT, unsigned int LED){
    *GPIO_PORT &= ~(LED);
}

int main(){
    unsigned int GPIO_PORT = 0;
    /* Bat LED1 va LED3 */

    enableLED(&GPIO_PORT, LED1 | LED3);
    if(GPIO_PORT & LED1){
        printf("LED1 is on\n");
    }
    if(GPIO_PORT & LED2){
        printf("LED2 is on\n");
    }
    if(GPIO_PORT & LED3){
        printf("LED3 is on\n");
    }
    /* Tat LED1 va bat LED2 */
    disableLED(&GPIO_PORT, LED1);
    enableLED(&GPIO_PORT, LED2);
    if(GPIO_PORT & LED1){
        printf("LED1 is on\n");
    }
    if(GPIO_PORT & LED2){
        printf("LED2 is on\n");
    }
    if(GPIO_PORT & LED3){
        printf("LED3 is on\n");
    }
    return 0;
}