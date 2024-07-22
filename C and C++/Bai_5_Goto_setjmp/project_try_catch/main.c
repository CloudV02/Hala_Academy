#include "try_catch.h"

/* Function Prototype of main */
int readFile(int data_flow);
int networkOperation(int Status_Network);
int calculateData(int Cal_Number);

int main(){
    
    TRY{
        printf("Complete readfile %d\n",readFile(1));
        printf("Complete network %d\n",networkOperation(1));
        printf("Complete calculate %d\n",calculateData(0));
    }
    CATCH(FILE_ERROR){
        printf("%s\n", error_message);
    }
    CATCH(NETWORK_ERROR){
        printf("%s\n", error_message);
    }
    CATCH(CALCULATION_ERROR){
        printf("%s\n", error_message);
    }

}


int readFile(int data_flow){

    if(data_flow == 0){
        THROW(FILE_ERROR,"Du lieu bi loi\n");
    }
    return 1;
}
int networkOperation(int Status_Network){
    if (Status_Network == 0)
    {
        THROW(NETWORK_ERROR,"Mang bi ngat\n");
    }
    return 1;
}
int calculateData(int Cal_Number){
    if (Cal_Number == 0)
    {
        THROW(CALCULATION_ERROR,"Loi tinh toan\n");
    }
    return 1;
}
