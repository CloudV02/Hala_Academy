#include <stdio.h>
#include <stdint.h>

/*Function Prototype*/
void Swap_Func(uint32_t *number_1, uint32_t *number_2);
void Cong(int a, int b);
void Tru(int a, int b);
void Nhan(int a, int b);
void Chia(int a, int b);
void Cal(void(*TinhToan)(int, int), int a, int b);

int main(){
	
	/*Pointer value */
	uint32_t value_a;
	
	uint32_t *ptr = &value_a;
	
	printf("%p\n", &value_a);
	printf("%p\n", ptr);
	
	float value_b;
	float *ptr_fl = &value_b;
	
	char value_c = 'A';
	char *ptr_char = &value_c;
	printf("%c\n", *ptr_char);
	
	printf("Dia chi cua func: %d\n", Tru);
	/*Pointer array*/
	
	uint32_t array[] = {0,1,2,3};
	
	uint32_t *ptr_array = array;
	
	printf("%p\n", array);
	printf("%p\n", ptr_array);
	
	/* Size of pointer */
	int *ptr_size_1 = NULL;
	double *ptr_size_2 = NULL;
	char * ptr_size_3 = NULL;
	
	printf("size of pointer: %d , %d, %d\n", sizeof(ptr_size_1), sizeof(ptr_size_2), sizeof(ptr_size_3));
	// Phu thuoc vao kien truc may tinh va trinh bien dich
	printf("size of interger: %d\n", sizeof(int));

	/* Application in swap function*/
	uint32_t number_1 = 10;
	uint32_t number_2 = 20;
	
	printf("*********Swap Func**********\n");
	printf("********Before Swapping**********\n");
	printf("Number 1: %d\n", number_1);
	printf("Number 2: %d\n", number_2);
	
	Swap_Func(&number_1, &number_2);
	printf("********After Swapping**********\n");
	printf("Number 1: %d\n", number_1);
	printf("Number 2: %d\n", number_2);
	
	/*Void Pointer tro toi bat ky dia chi nao, ma khong can biet kieu du lieu gia tri cua dia chi do*/
	
	void *ptr_void;
	int value_int = 10;
	char value_char = 'a';
	
	ptr_void = &value_int;
	printf("Con tro void int: %d\n",*(int*)ptr_void); // ep kieu con tro void -> (int*)void
	
	ptr_void = &value_char;
	printf("Con tro void char: %c\n",*(char*)ptr_void);
	
	/*Function pointer + void pointer*/
	void (*TinhToan)(int, int);
	Cal(Cong,5,6);
	Cal(Tru,5,10);
	Cal(Nhan,5,8);
	Cal(Chia,5,5);
	
	/*Function pointer array*/
	void(*arr_func[])(int , int ) = {Cong, Tru, Nhan, Chia};
	printf("*********Function Pointer Array********\n");
	arr_func[0](1,3);
	
	/*Pointer to Constant*/
	printf("*******Pointer to Constant **********\n");
	int val_cons = 10;
	int const *ptr_cons = &val_cons; // const int *ptr_cons
	printf("Val of pointer to constant: %d\n", *ptr_cons);
	
	//*ptr_cons = 15; bien nay khong the thay doi gia tri cua dia chi
	val_cons = 20; // su dung bien duoc tro toi de thay doi
	printf("Val of pointer to constant after change: %d\n",*ptr_cons);
	
	/*Cosntant Pointer*/
	printf("******Constant Pointer**********\n");
	int cons_val = 50;
	int test_val = 10;
	int *const const_ptr = &cons_val;
	//const_ptr = &test_val; khong the tro toi dia chi khac 
	
	
}

/*Swap function*/
void Swap_Func(uint32_t *number_1, uint32_t *number_2){
	uint32_t swap;
	swap = *number_1;
	*number_1 = *number_2;
	*number_2 = swap;
	
}

/*Function pointer*/
void Cong(int a, int b){
	printf("Tong cua %d va %d = %d\n",a,b,a+b);
}
void Tru(int a, int b){
	printf("Hieu cua %d va %d = %d\n",a,b,a-b);
}
void Nhan(int a, int b){
	printf("Tich cua %d va %d = %d\n",a,b,a*b);
}
void Chia(int a, int b){
	printf("Thuong cua %d va %d = %.2f\n",a,b,(float)a/b);
}
void Cal(void (*TinhToan)(int , int), int a, int b){
	/*Co 2 cach goi func pointer -- (*TinhToan)() -- TinhToan()*/
	printf("Calculate ................\n");
	TinhToan(a,b);
	
}
