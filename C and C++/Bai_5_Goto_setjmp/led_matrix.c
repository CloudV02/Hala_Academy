#include <stdio.h>
#include <stdint.h>
typedef enum{
    FIRST,
    SECOND,
    THIRD
} Sentence;


void delay(){
    for (uint64_t start = 0; start < (uint64_t)(60000000); start++);
}
int letter_A[8][8] = {  {0,0,1,0,0,0,0,0},
                        {0,1,0,1,0,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,1,1,1,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0}, };
int letter_H[8][8] = {  {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,1,1,1,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},
                        {1,0,0,0,1,0,0,0},  };

int letter_L[8][8] = {  {1,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0},
                        {1,1,1,1,1,0,0,0},  };

char first_sentence[] = "HELLO";
char second_sentence[] = "FASHION SUIT";
char third_sentence[] = "SUITABLE PRICE";

char letter;

int button = 0;

int main(){
    Sentence sentence = FIRST;
    while(1){
        switch (sentence)
        {
        case FIRST:
            for (uint32_t index = 0; index < sizeof(first_sentence); index++)
            {
                if (first_sentence[index]=='H')
                {
                    for(int i = 0; i<8 ; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (letter_H[i][j]==1)
                            {
                                printf("Led [%d][%d] sang\n",i,j);
                                if(button == 1){
                                    goto exit_loop;
                                }
                            }
                        }
                        
                    }
                }

                if(first_sentence[index]=='E'){
                    // in ra chu e
                }
            }
            printf("First sentence is done\n");
            delay();
            goto logic;
            
        case SECOND:
            for (uint32_t index = 0; index < sizeof(second_sentence); index++)
            {
                if (second_sentence[index]=='A')
                {
                    for(int i = 0; i<8 ; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (letter_A[i][j]==1)
                            {
                                printf("Led [%d][%d] sang\n",i,j);
                                if(button == 1){
                                    goto exit_loop;
                                }
                            }
                        }
                        
                    }
                }

                if(second_sentence[index]=='F'){
                    // in ra chu e
                }
            }
            printf("Second sentence is done\n");
            delay();
            goto logic;
        case THIRD:
            for (uint32_t index = 0; index < sizeof(third_sentence); index++)
            {
                if (third_sentence[index]=='I')
                {
                    for(int i = 0; i<8 ; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (letter_L[i][j]==1)
                            {
                                printf("Led [%d][%d] sang\n",i,j);
                                if(button == 1){
                                    goto exit_loop;
                                }
                            }
                        }
                        
                    }
                }

                if(third_sentence[index]=='S'){
                    // in ra chu e
                }
            }
            printf("Third sentence is done\n");
            delay();
            goto logic;

        }

        logic:
            if (sentence == FIRST)
            {
                sentence = SECOND;
            }
            else if (sentence == SECOND)
            {
                sentence = THIRD;
            }
            else if (sentence == THIRD){
                sentence = FIRST;
            }
            else{
                printf("Error !!!\n");
                goto exit_loop;
            }
            goto exit;
        exit_loop:
            printf("Stop !!");
            break;
        exit:;
    }
}