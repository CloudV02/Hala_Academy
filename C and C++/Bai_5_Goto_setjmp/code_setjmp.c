#include <stdio.h>
#include <setjmp.h>

jmp_buf buf;

double thuong(int a, int b){
    if(b == 0){
        longjmp(buf,1);
    }
    return a/(double)b;
}

int checkArray(int arr[], int size){
    if(size <= 0){
        longjmp(buf,2);
    }
}

int main(){
    int exception_code = setjmp(buf);

    if (exception_code == 0)
    {
        double ketqua = thuong(8,3);
        printf("%f\n",ketqua);

        int array[0];
        checkArray(array,sizeof(array));
    }
    else if(exception_code == 1){
        printf("ERROR! Mau = 0\n");
    }
    else if(exception_code == 2){
        printf("ERROR! Size array <= 0\n");
    }
}