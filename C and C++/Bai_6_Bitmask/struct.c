#include <stdio.h>
#include <stdint.h>

#define ENABLE 1
#define DISABLE 0

typedef struct 
{
    uint8_t LED1 :1;
    uint8_t LED2 :1;
    uint8_t LED3 :1;
    uint8_t LED4 :1;
    uint8_t LED5 :1;
    uint8_t LED6 :1;
    uint8_t LED7 :1;
    uint8_t LED8 :1;
}LEDStatus;

void displayAllStatusLed(LEDStatus status){
    uint8_t *statusPtr = (uint8_t*)&status;
    for (int i = 0; i < 8; i++)
    {
        printf("LED%d: %d\n",i+1,((*statusPtr)>>i)&1);
    }
    
}

int main(){
    LEDStatus ledStatus = {.LED7 = ENABLE, .LED5 = ENABLE};

    // Bat LED 1 va 3
    ledStatus.LED1 = ENABLE;
    ledStatus.LED3 = ENABLE;

    displayAllStatusLed(ledStatus);

    return 0;
}