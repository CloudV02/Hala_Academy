#include <stdio.h>
#define CH_TRINH 2
void delay(double second){
    double start;
    for(start = 0; start < second * 6000000; start++);
}
typedef enum{
    RED,
    YELLOW,
    GREEN


}TrafficLightState;
int main(){
    /* Vi du 1 */
    #if CH_TRINH == 1

    int i = 0;
    
    start: // khi goi goto start se quay ve lable start
    if(i>=5){
        goto end;   
    }
    
    i++;
    printf("%d\n",i);
    goto start;

    end: // khi goi goto end se quay ve lable start
    printf("Ket thuc %d",i);


    /* Vi du 2 */
    #elif CH_TRINH == 2
    TrafficLightState Current_State = RED;
    while(1){
        switch (Current_State)
        {
        case RED:
            printf("RED\n");
            delay(50);
            Current_State = GREEN;
            goto exit_state;
        case YELLOW:
            printf("YELLOW\n");
            delay(20);
            Current_State = RED;
            goto exit_state;
        case GREEN:
            printf("GREEN\n");
            delay(100);
            Current_State = YELLOW;
            goto exit_state;
        }
        exit_state:;
    }


    #endif

}