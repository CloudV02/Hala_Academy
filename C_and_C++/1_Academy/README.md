<details>
<summary> <h1>COMPILER AND MACRO </h1> </summary>

<details>
<summary> <h2> 1. Compiler </h2> </summary>

là quá trình biên dịch từ mã người dùng sang mã máy. Nó sẽ bao gồm các bước Preprocessor, Compilation, Assembler, Linking
- Quá trình Preprocessor: Là quá trình tiền xử lý sẽ gộp các file source với file thư viện tương ứng (file.c và file.h), và xử lý các chỉ thị tiền xử lý như macro và xóa các command line
                        gcc -E main.c -o main.i
- Quá trình Compilation: là quá trình biên dịch chương trình sang ngôn ngữ hay file assembly
                        gcc -S main.i -o main.s
- Quá trình Assembler: Quá trình này sử dụng trình biên dịch Assembly thay vì Compiler, nó gộp các file.s nếu có và chuyển sang file.o file object tương ứng
                        as main.s -o main.o
- Quá trình Linking: Là quá trình ta gộp tất cả các file.o vào để tạo ra file cuối cùng để thực thi
                        gcc -v -o main main.o //(thêm các file.o nối đuôi main.o nếu có)

Ví dụ chương trình dưới đây

**main.c**
```C
#include "func1.h"
int main()
{
    In_Tong(5,6);
}
```

**func1.c**
```C
#include "func1.h"
void In_Tong(int a, int b){
    printf("%d\n",a+b);
}
```
**func1.h**
```C
#ifndef __FUNC_H__

#define __FUNC_H__


#include <stdio.h>

void In_Tong(int a, int b);

#endif
```
- Giải thích: Chương trình main sẽ là chương trình thực thi , chương trình func1.h sẽ lưu các thư viện và các nguyên mẫu hàm của chương trình func1.c, chương trình func1.c sẽ chưá các hàm cung cấp cho chương trình khác.
Bắt đầu với quá trình Processing khi ta sử dụng gcc -E main.c -o main.i trong command line thì file main.i sẽ được tạo ra, nội dung include func1.h bao gồm cả các #include trong file func1.h cũng được hết ra ở file main.i 
**file main.i**
```C
#nội dung stdio.h
....
....
# 7 "func1.h" 2


# 8 "func1.h"
void In_Tong(int a, int b);
# 2 "main.c" 2


int main()
{
    In_Tong(5,6);
}
```
Tiếp theo là quá trinhf Compiler ta sẽ sử dụng gcc -S main.i -o main.s để tạo ra 1 file assembly nó sẽ gần với ngôn ngữ máy -> máy tính của chúng ta sẽ hiểu được ngôn ngữ đấy, quá trình này giôngs như là người phiên dịch, là cầu nối giữa file.c người dùng và file.o(file mã máy). Sau khi thực thi main.s sẽ có nội dung như sau:

**file main.s**
```S
	.file	"main.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$16, %esp
	call	___main
	movl	$6, 4(%esp)
	movl	$5, (%esp)
	call	_In_Tong
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE10:
	.ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
	.def	_In_Tong;	.scl	2;	.type	32;	.endef

```
Tiếp theo là quá trình assembler quá trình này ta sẽ sử dụng trình biên dịch của ngôn ngữ Assembly biên dịch ra mã máy file.o -> đây là mã mà máy tính dùng để chạy
as main.s -o main.o. Vì đây là ngôn ngữ của mã máy nên cta không hiểu gì hết :)) nên để người dungf dễ hiểu từ đó mới sinh ra các file như file.hex

**file main.o**
```C
    L     ‚       .text           ,   Ü   d           0`.data                               @ 0À.bss                                € 0À/4              $                 @ 0@/15             8   ,  x         @ 0@U‰åƒäðƒìè    ÇD$   Ç$   è    ¸    ÉÃGCC: (MinGW.org GCC-6.3.0-1) 6.3.0         zR |ˆ           )    A…B
eÅ  
                      .file       þÿ  gmain.c            _main                            .text          )                .data                            .bss                                           #                     $          8                ___main          _In_Tong         .   .rdata$zzz .eh_frame .rdata$zzz .eh_frame 
```
Tiếp theo là quá trình Linking: Như ta thấy ở 3 chương trình ví dụ thì file main.c vẫn chưa thể chạy đúng dù có phần nguyên mẫu hàm của chương trình fun1.c ở trong file func1.h nhưng nội dung của hàm In_Tong() ta vẫn chưa biết và nó nằm ở trong chương trình func1.c. vậy nên ta mới cần quá trinhf Linking để liên kết 2 chương trình vào với nhau gcc -v -o main main.c func1.o -> tạo ra file main.exe. Để tạo ra file.o nhanh ta có thể sử dụng gcc -c func1.c -o func1.o
**Kết quả**
```
11
```
</details>

<details>
<summary> <h2> 2. Macro </h2> </summary>
Là các chỉ thị tiền xử lý được xử lí trong quá trình Preprocessor gồm:
- #include -> để thêm nội dung các file khác vào chương trình thường là các source .h

**Ví dụ**
```C
    #include <stdio.h>
    #include "test.txt"
    ...
```
- #define -> Dùng để thay thế nội dung này thành 1 nội dung khác mà mình định nghĩa trước khi chương trình biên dịch
**Ví dụ**
```C
#define a 100
#define Tong_Func(a,b)   \
printf("tong 2 so = %d\n",a+b);
....
```
- #ifdef, #ifndef, #if, #elif, #else -> Với #if nếu điều kiện if đúng tức là đã được define trước đó đúng với điều kiện thì mã nguồn dưới if sẽ được thực thi, tương tự với #elif và #else. Còn với #ifdef và ifndef -> thì nếu define đấy mà chưa được định nghĩa thì khi sẽ thực thi mã nguồn dưới ifndef. Tương tự với ifdef nếu define đó được định nghĩa rồi thì chạy mã lệnh bên dưới. Cả 2 gặp #endif là sẽ kết thúc
**Ví dụ ifdef và ifndef**
```C
#include <stdio.h>
#define __FUNC__

#ifdef __FUNC__
    void test(){
        printf("Hello");
    }

#endif

#ifndef __FUNC__
    void test(){
        printf("Hi");
    }

#endif
int main()
{
    test();
}

```
**Ví dụ #if # elif #else**

```C
#include <stdio.h>
#define DEFINE_1    0
#define DEFINE_2    1
#define DEFINE_3    2
#define CALL_DEFINE DEFINE_1
#if(CALL_DEFINE == DEFINE_1)
    void test(){
        printf("Define 1");
    }
#elif(CALL_DEFINE == DEFINE_2)
    void test(){
        printf("Define 2");
    }
#else
    void test(){
        printf("Define 3");
    }
#endif
int main()
{
    test();
}
```

- Macro không phải kiểu dữ liệu nào cả mà cũng có không có kích thước cụ thể vì nó xảy ra ở quá trình tiền xử lý và nó thay thế cái đoạn được định nghĩa vào

- Dùng macro giúp chương trình chạy nhanh hơn nhưng size của chương trình sẽ tăng. Còn dùng function nó sẽ xảy ra quá trình Function Call làm chậm chương trình đặc biệt với funcion được gọi nhiều lần

- Dùng ## để nối chuoi trong macro
**Ví dụ**
```C
#define CREATE_VAR(name, number) \
int int_##name = number; \
char char_##name; \
double double_##name
```
- Va_args được sử dụng khi không xác định được tham số đầu vào trong macro. Cấu trúc được viết (...) __VA_ARGS__
```C
#define ARRAY(number,...) \
int array(number) = {__VA_ARGS__};
```
</details>
</details>

<details>
<summary> <h1>BÀI POINTER</h2></summary>

==============
<details>
<summary> <h2>1. Khái niệm về Pointer</h2></summary> 
- Pointer là 1 biến chứa địa chỉ của 1 biến khác. Pointer có kích thước dựa vào kiến trúc máy tính và trình biên dịch.
VD: Với kiến trúc trên máy tính laptop 64bit thì size của sizeof(pointer) = 8 byte, tương tự với kiến trúc 32bit sẽ là 4 byte
```C
    int *ptr_size_1 = NULL;
	double *ptr_size_2 = NULL;
	char * ptr_size_3 = NULL;
	
	printf("size of pointer: %d , %d, %d\n", sizeof(ptr_size_1), sizeof(ptr_size_2), sizeof(ptr_size_3));
	// Phu thuoc vao kien truc may tinh va trinh bien dich
	printf("size of interger: %d\n", sizeof(int));
```
*** Kết quả: ***
```C
    size of pointer: 4 , 4, 4
    size of interger: 4
```
- Kiểu dữ liệu khai báo biến pointer sẽ là kiểu dữ liệu của giá trị tại địa chỉ đang được trỏ tới.
</details>

<details>
<summary><h2>2. Các loại Pointer</h2></summary>

- **Void Pointer**: Dùng để trỏ tới 1 địa chỉ mà không cần biết kiểu dữ liệu của giá trị tại địa chỉ được trỏ tới. Nhưng muốn sử dụng giá trị trong địa chỉ đó thì phải ép kiểu về đúng kiểu dữ liệu của giá trị. Được sử dụng khi không xác định được kiểu dữ liệu được trỏ tới


**Cách khai báo:**
```C
    void *ptr_void;
	int value_int = 10;
	char value_char = 'a';
	
	ptr_void = &value_int;
	printf("Con tro void int: %d\n",*(int*)ptr_void); // ep kieu con tro void -> (int*)void
	
	ptr_void = &value_char;
	printf("Con tro void char: %c\n",*(char*)ptr_void);
```
Sau khi ép kiểu nó sẽ về ra đúng kết quả.

VD: Như trong hình dưới đây sẽ là 1 function pointer với 2 tham số đầu vào const void * . Con trỏ void được sử dụng ở đây vì ta đang không biết tham số được truyền vào là gì, ở trong ví dụ thì nó đang muốn lấy các giá trị của các phần tử trong struct mà đối với struct thì nó sẽ có nhiều kiểu dữ liệu bên trong struct.

```C
void sort(SinhVien array[], size_t size, int (*compareFunc)(const void *, const void *)) {
   int i, j;
   SinhVien temp;
   for (i = 0; i < size-1; i++)   
       for (j = i+1; j < size; j++)
           if (compareFunc(array+i, array+j)>0) {
               temp = array[i];
               array[i] = array[j];
               array[j] = temp;
           }
}
```

- **Function Pointer** : Dùng để trỏ tới 1 hàm trong chương trình. Nó thường được sử dụng khi ta muốn 1 hoặc nhiều hàm làm tham chiếu của 1 hàm khác -> Dùng để viết thư viện.
Như ví dụ dưới đây ta có thể thấy sử dụng khai báo 1 function pointer chưa trỏ tới địa chỉ nào cả, và nó sẽ tham sôs cho hàm Cal để khi ta muốn trỏ tới hàm nào chỉ cần nhập hàm đó vào, và biến function pointer sẽ trỏ tới địa chỉ của hàm đó và thực thi.

```C
void (*TinhToan)(int, int);
	Cal(Cong,5,6);
	Cal(Tru,5,10);
	Cal(Nhan,5,8);
	Cal(Chia,5,5);
```
```C
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
```

- **Pointer to Constant** :  Định nghĩa 1 con trỏ không thể thay đổi giá trị của địa chỉ được trỏ đến. Muốn thay đổi được giá trị phải thay đổi đúng cái biến chứa giá trị đó.
Như trong đoạn code dưới đây ta khai báo 1 biến val lưu 1 giá trị 10 và 1 con trỏ pointer to constant trỏ tới địa chỉ của biến val. Nhưng cái khác với các biến bình thường khi ta truy xuất tới giá trị bằng con trỏ *ptr_cons để thay đổi giá trị thì trình biên dịch sẽ báo lỗi. Việc này sẽ ứng dụng vào việc ta muốn đưa 1 giá trị vào hàm và chỉ cho phép đọc, không cho phép thay đổi giá trị đó.

```C
    printf("*******Pointer to Constant **********\n");
	int val_cons = 10;
	int const *ptr_cons = &val_cons; // const int *ptr_cons
	printf("Val of pointer to constant: %d\n", *ptr_cons);
	
	//*ptr_cons = 15; bien nay khong the thay doi gia tri cua dia chi
	val_cons = 20; // su dung bien duoc tro toi de thay doi
	printf("Val of pointer to constant after change: %d\n",*ptr_cons);
```

- **Constant Pointer**: Định nghĩa 1 con trỏ địa chỉ nó trỏ tới sẽ không thể thay đổi. Tức là biến con trỏ này sẽ không thể trỏ tới 1 địa chỉ khác mà chỉ sử dụng dc 1 địa chỉ ban đầu.
Như ví dụ dưới đây khi ta khai báo 1 constant pointer và cho nó 1 địa chỉ thì khi nó thay đổi trình biên dịch sẽ báo lỗi. Ứng dụng trong nhúng vì địa chỉ trong các vi điều khiển thường được fix cứng. Đặc biệt như các hàm ngắt nó phải trỏ tới đúng địa chỉ cần đến, nếu thay đổi địa chỉ thì sẽ bị sai chương trình

```C
    printf("******Constant Pointer**********\n");
    int cons_val = 50;
	int test_val = 10;
	int *const const_ptr = &cons_val;
	//const_ptr = &test_val; khong the tro toi dia chi khac
```
</details>
</details>

<details>
<summary><h1>BÀI CÁC BIẾN ĐẶC BIỆT</h1></summary>


<details>
<summary><h2>1. Extern</h2></summary>
- Dùng extern để lấy 1 biến hoặc 1 hàm được khai báo global của nguồn file khác vào chương trình hiện tại của mình để sử dụng
- Giúp cho chương trình có thể tách thành các phần nhỏ để dễ dàng quản lý

**Khai báo chương trình 1**
```C
	#include <stdio.h>

extern int a;
extern void cong(int a, int b);

int main(){
    printf("%d\n",a);
    cong(5,6);
}
```
**Khai báo chương trình 2**
```C
#include <stdio.h>
int a = 10;

void cong(int a, int b){
    printf("%d", a+b);
}
```
Ở ví dụ này chương trình 1 sẽ lấy biến a và cả hàm void cong của chương trình 2 để dùng bằng việc sử dụng biến extern để lấy. Ứng dụng của việc chia file để mỗi chương trình sẽ có 1 nhiệm vụ riêng, và biến extern khi cần thiết sẽ lấy 1 biến hoặc hàm trong chương trình riêng đó để sử dụng
Để chương trình tìm được biến của chương trình 2 phải linking với nhau gcc chuongtrinh1 chuongtrinh2 -o main

**Kết quả**
```C
	10
	11
```
</details>

<details>
<summary><h2>2. Static</h2></summary>

**Static local**
+ Sẽ được khai báo trong 1 hàm và sẽ được cấp phát địa chỉ cho biến đó. Thông thường nếu không sử dụng local static cho biến đó nó sẽ được lưu trong vùng stack và khi thoát khỏi hàm sẽ biến mất, còn nếu sử dụng local static nó sẽ tồn tại trong suốt time chạy chương trình và có 1 địa chỉ cụ thể ở vùng bss nhưng biêns local static chỉ được sử dụng trong hàm mà khai báo biến đó. Nếu muốn sử dụng biến đó bên ngoài hàm, phải cần 1 biến con trỏ global trỏ tới địa chỉ của local static

**Chương trình**
```C
#include <stdio.h>
void tong(){
    /* variable */
    int a = 0;
    a++;
    printf("bien thuong: %d\n",a);
    /* static variable */
    static int c = 0;
    c++;
    printf("bien static: %d\n",c);
}
int main(){
    tong();
    tong();
    tong();
}
```
Vói chương trình trên ta khai báo 1 biến local thông thường và 1 biến local static. Thì khi ra khỏi hàm kết quả nhận được như bên dưới. Thấy rằng giá trị của biến thường không thay đổi vì mỗi lần vào hàm nó sẽ khởi tạo lại vùng nhớ 1 lần nên khi dc cộng nên vẫn chỉ = 1. Còn đối với biến static khi gọi hàm lần đầu nó đã cấp phát cho biến đó 1 địa chỉ cố định nên khi chạy lại hàm nó thấy biến đó đã được khởi tạo và nó chỉ cần truy xuất lấy giá trị thực thi nên nó có thể thay đổi vì nó đã có địa chỉ trước đó. Nhưng biến static int c = 0; chỉ dùng được trong hàm nó được khai báo, muốn sử dụng nó phải có 1 con trỏ trỏ tới nó để lấy địa chỉ, lúc đó mới mang ra ngoài hàm để sử dụng
**Kết quả**
```
	bien thuong: 1
	bien static: 1
	bien thuong: 1
	bien static: 2
	bien thuong: 1
	bien static: 3
```

- **Static global**: 
+ Nó sẽ được coi như là 1 biến global thông thường trong file nguồn hiện tại, điểm khác duy nhất là các file nguồn khác không thể sử dụng extern để lấy biến đó.
Như ví dụ dưới đây. Như ta thấy chương trình 1 extern lấy biến a của chương trình 2. Nhưng biến a của chương trình 2 là biến global static nên khi extern cho biến static này sẽ bị báo lỗi. Ứng dụng của biến static sẽ là không muốn cái biến đó được sử dụng ở 1 chương trình khác, để bảo vệ code.
**Chương trình 1**
```C
#include <stdio.h>
extern int a;
extern void cong(int a, int b);
int main(){
    printf("%d\n",a);
    cong(5,6);
}
```
**Chương trình 2**
```C
#include 
static int a = 10;
void cong(int a, int b){
    printf("%d", a+b);
}
```
**Kết quả**
```
C:\Users\ASUS\AppData\Local\Temp\ccugdRGH.o:program_1.c:(.text+0xf): undefined reference to `a'
collect2.exe: error: ld returned 1 exit status
```
</details>

<details>
<summary><h2>3. Register</h2></summary>
- Từ khóa này để cho người lập trình muốn cho 1 biến thường xuyên sử dụng được lưu trữ trong thanh ghi để sử dụng thay vì biến đos được lưu vùng nhớ RAM -> việc này làm tăng tốc độ xử lý của biến đó
Như ví dụ dưới đây nếu ta bỏ từ khóa register ở biến i chương trình sẽ chạy lâu hơn so với thêm register, dù thời gian không đáng kể đối với máy cấu hình mạnh như lap. Nhưng đối với vi điều khiển nó có tốc độ xử lý chậm, thì việc sử dụng từ khóa register hợp lí sẽ giảm thời gian chuyển đổi của biến từ RAM tới ALU bằng việc lưu tại Register của vi điều khiển.

**Chương trình ví dụ**
```C
#include <stdio.h>
#include <time.h>

int main() {
    // Lưu thời điểm bắt đầu
    clock_t start_time = clock();
    register int i;

    // Đoạn mã của chương trình
    for (i = 0; i < 2000000; ++i) {
        // Thực hiện một số công việc bất kỳ
    }

    // Lưu thời điểm kết thúc
    clock_t end_time = clock();

    // Tính thời gian chạy bằng miligiây
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Thoi gian chay cua chuong trinh: %f giay\n", time_taken);

    return 0;
}

```
**Kết quả**
```
Trước khi sử dụng từ khóa Register biến i: 0.0003000 giây
Sau khi sử dụng từ khóa Register biến i: 0.0000000 giây
```

4. Volatile
- Từ khóa này để cho trình biên dịch của chương trình luôn chạy dòng code đó vì nhiều trình biên dịch sẽ tối ưu hóa đoạn code là chỉ chạy 1 lần -> dẫn tới sai sót trong hệ thống nhúng, vì hệ thống nhúng có những exception nên khi quay về chương trình chính sẽ tiếp tục -> có thể gây sai sót. Khi dùng volatile giúp trình biên dịch hiểu là phải chạy dòng code đó lại.

</details>
</details>

<details>
<summary><h1>BÀI GOTO SETJUMP</h1></summary>

1. Goto
- Goto cho phép chương trình nhảy từ lệnh goto đến 1 label đã được đặt trước trong cùng 1 hàm.
- Sử dụng goto giúp thực hiện chương trình nhanh gọn, nhưng sử dụng nhiều khiến chương trình khó quản lý. Nên vì vậy chỉ nên dùng goto với những chương trình có nhiều vòng lặp
Ví dụ về goto: Đây là chương trình chạy đến khi lớn hơn = 5 thì chương trình sẽ kết thúc. Đầu tiên nó sẽ chạy đi qua hàm if vì lúc này i = 0, rồi lúc này nó sẽ gặp lệnh goto start, lệnh này sẽ gọi ra cái label start và chương trình sẽ nhảy đến lable và tiếp tục chương trình, cho đến khi i>=5 nó mới vào điều kiện if và gặp goto end để nhảy tới lable end và lable end này nằm ở cuối chương trình, nó sẽ thực hiện nốt các lệnh còn lại và kết thúc chương trình.
**Chương trình**
```C
#include <stdio.h>
int main(){
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
}
```

2. Setjmp.h
- Là thư viện trong ngôn ngữ C. Thư viện này cung cấp 2 hàm setjump và longjmp. Các hàm này để xử lý ngoại lệ trong C
- Nó khác goto ở chỗ có thể thay đổi điều kiện giống if và nó có thể khai báo toàn cục, trong khi Goto chỉ sử dụng trong cùng 1 hàm 
- Thường dùng để báo lỗi trong chương trình

Giải thích ví dụ: Để sử dụng các hàm setjump và longjmp ta phải khai báo thư viện setjump. Đầu tiên ta sẽ khai báo 1 biến jmp_buf buf, kiểu dữ liệu này được define trong thư viện setjmp, nó là tham số để điều chỉnh giá trị của hàm setjump(). Khi vào chương trình thì giá trị của hàm setjmp(buf) sẽ luôn là 0, muốn thay đổi giá trị của hàm này phải chạy đến hàm longjmp(buf,...) ... ở đây là 1 giá trị int nào đó. Và lúc đó hàm setjmp sẽ thay đổi giá trị dựa vào cái longjmp.
Và nhờ việc biến buf là biến global nên vào ta có thể tạo ra các ngoại lệ ngắt chương trình. Như trong chương trình bên dưới ta sẽ có 1 ngoại lệ trong hàm nếu điều kiện đúng nó sẽ thoát ra khỏi hàm nhảy tới setjmp luôn mà không chạy tiếp cả hàm đó. Ứng dụng lớn nhất của thư viện setjmp.h là tạo ra các TRY CATCH THROW như ở trong Java hoặc Exception trong Python.
**Chương trình ví dụ**
```C
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
```
</details>

<details>
<summary><h1>BÀI BIT MASK</h1></summary>

Bitmask là 1 kỹ thuật trong lập trình sử dụng các phép toán bit như AND OR XOR NOT SHIFT để thực hiện các trạng thái như thiết lập, hoặc xóa hoặc kiểm tra các bit cụ thể trong 1 Byte

## Toán tử bitwise
1. AND bitwise (&)
Là toán tử thực hiện phép AND bitwise cho các cặp bit của 2 số. Kết quả là 1 nếu 2 bit tương ứng là 1, chỉ cần 1 bit là 0 sau khi thực hiện AND bitwise sẽ ra 0. Giống như việc nhân 2 bit 
Như ví dụ bên dưới kết quả phép AND là 100 sau khi AND 125 & 100 , tương ứng sẽ là (0111 1101) & (0110 0100) = 110 0100 = 100

2. OR bitwise (|)
Là toán tử thực hiện phép OR bitwise cho các cặp bit của 2 số. Kết quả là 1 nếu 1 trong 2 bit OR với nhau là 1, và kết quả là 0 khi 2 bit tương ứng đều = 0. Giống như phép cộng giữa 2 bit
Ví dụ bên dưới kết quả OR là 125 sau khi OR 125 | 100, tương ứng (0111 1101) | (0110 0100) = 0111 1101 = 125

3. XOR bitwise (^)
Là toán tử thực hiện phép XOR bitwise cho các cặp bit của 2 số. Kết quả là 1 nếu cặp bit tương ứng khác nhau, nếu giống nhau kết quả là 0
Ví dụ bên dưới kết quả XOR là 25 sau khi XOR 125 ^ 100, tương ứng (0111 1101) ^ (0110 0100) = 0001 1001 = 25

4. NOT bitwise(~)
Dùng để thực hiện phép NOT bitwise trên từng bit của 1 số. Nó sẽ đảo trạng thái của bit đó nếu bit đó là 1 sẽ thành 0, 0 thành 1
Ví dụ bên dưới ~(125) = 130 tương ứng ~(0111 1101) = (1000 0010) = 130, ~(100) = 155 tương ứng ~(0110 0100) = (1001 1011) = 155

5. Shift left (<<)
Để dịch bit sang trái, các bit sẽ dịch sang trái và phần bị dịch mất mà không có thông tin bit trước đó sẽ về 0
Ví dụ (125) << 2 tương ứng (0111 1101) << 2 = 1111 0100 (244)

6. Shift right (>>)
Để dịch bit sang phải, các bit sẽ dịch sang phải và phần bị dịch mất mà không có thông tin bit trước đó sẽ về 0
Ví dụ (100) >> 2 tương ứng (0110 0100) >> 2 = 0001 1001 (25)

```C
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
```
**Kết quả**
```
100
125
25
130
155
244
25
```

</details>

<details>
<summary><h1>BÀI STRUCT UNION</h1></summary>

## 1. STRUCT
- Struct là 1 kiểu cấu trúc dữ liệu do người lập trình tự định nghĩa bằng cách nhóm các biến có kiểu dữ liệu khác nhau lại với nhau. Việc có kiểu struct giúp tạo ra 1 thực thể dữ liệu lớn hơn và được tổ chức chặt chẽ.
- Mỗi biến trong struct gọi là 1 thành viên hoặc trường
VD: Thì x và y ở đây là thành viên của struct Test
```C
struct Test{
    int x;
    int y;
}
```
- Kích thước của struct sẽ dựa vào kiểu dữ liêu, kích thước của các thành viên và ngoài ra còn phụ thuộc vào Alignment để căn chỉnh tại các địa chỉ bộ nhớ cụ thể, align này sẽ dưạ vào kiểu dữ liệu lớn nhất trong struct. Để đảm bảo alignment thì khoảng trôngs có thể được thêm vào giữa các thành phần của struct. Điều này khiến cho kích thước của struct luôn lớn hơn nếu như ta tính theo thông thường
- Như ví dụ dưới đây ta sẽ tính toán kích thước của struct. Đầu tiên xác định được kiểu dữ liẹu lớn nhất là u32 -> aligment sẽ là 4 byte 
+ u8 var2[9] -> 9 byte sẽ được sử dụng mà aligment ở đây là 4 byte -> byte cuối cùng phải thêm padding để đúng với alignment của struct-> 8 byte + 1 byte + 3 byte padding
+ uint16_t var1[10] -> ta có u16 là kiểu dữ liệu chiếm 2 byte -> phần tử đầu tiên vẫn đủ số byte thêm vào padding -> 9 phần tử con lại với kiểu u16
-> 9 * 2byte = 18 byte nhưng alignment = 4byte -> sẽ là 20 byte -> padding 2 byte -> 18 byte + 2 byte padding
+ uint32_t var3[2] -> u32 là kiểu dữ liệu chiêms 4 byte -> đúng alignment -> chiếm 8byte
-> struct frame này có kích thước là 40 byte
**Ví dụ**
```C
#include <stdio.h>
#include <stdint.h>
typedef struct{
    uint8_t var2[9];
    uint16_t var1[10];
    uint32_t var3[2];

}frame;
int main(){
    printf("%d", sizeof(frame));
}
```
**Kết quả**
```C
    40
```

## 2. Union
- Union cũng là 1 kiểu dữ liệu do người lập trình tự định nghĩa bằng cách nhóm nhiều kiểu dữ liệu vào với nhau. Nhưng khác với struct thì các biến trong union sẽ chia sẻ cùng 1 vùng nhớ, tức là các biến trong union sẽ có 1 địa chỉ giống nhau, điều này sẽ giúp tiết kiệm bộ nhớ. Điều này cũng có nghĩa trong 1 thời điểm chỉ có 1 thành viên trong union được sử dụng nếu sử dụng nhiều hơn 1 thành viên có thể bị sai dữ liệu mình mong muốn. Ứng dụng của union thường ứng dụng vào nhúng, khi mà trong nhúng chỉ cần tác động từng bit không cần phải tác động cả 1 biến byte. Ngoài ra kiểu cấu trúc alingment giống như trong struct, nhưng kích thước sẽ dựa vào biến có kích thước  lơns nhất trong union
- Ta sẽ phân tích ví dụ bên dưới để hiểu rõ cách share vùng nhớ của union. Ở union frame var2 có kiểu dữ liệu lơn nhất nó sẽ luôn lấy dữ liệu của toàn bộ bit tức là 32bit, var1 có kiểu dữ liệu là u8 nó sẽ chỉ lấy 8bit đầu, var3 thì có kiểu dữ liệu là u16 và nó lấy 16bit đầu
+ Ta truyền vào 1 số 4668481 tương ứng 0100 0111 0011 1100 0100 0001 lúc này biến var1 sẽ lấy 8 bit đầu 0100 0001 = 65, var2 sẽ lấy toàn bộ bit 0100 0111 0011 1100 0100 0001 = 4668481, var3 sẽ lấy 16 bit đầu 0011 1100 0100 0001 = 15,425

**Ví dụ**
```C
#include <stdio.h>
#include <stdint.h>

typedef union{ 
    uint8_t  var1; //  0 - 255
    uint32_t var2; //  0 - 4294967295
    uint16_t var3; //  0 - 65535
} frame;

int main(){
    frame data;
    //data.var1 = 5;
    data.var2 = 4668481; // 0100 0111 0011 1100 0100 0001
    //data.var3 = 7;

    printf("Data.var1 = %d\n", data.var1);
    printf("Data.var2 = %u\n", data.var2);
    printf("Data.var3 = %d\n", data.var3);
}
```
**Kết quả**
```
    Data.var1 = 65
    Data.var2 = 4668481
    Data.var3 = 15425
```
</details>

<details>
<summary><h1>BÀI MEMORY LAYOUT</h1></summary>

Các file sau quá trình build processing sẽ tạo ra các file thực thi như file.exe trên window hoặc như file.hex trên vi điều khiển để lưu vào bộ nhớ như SSD hoặc Flash. Sau đó khi chạy chương trình nó sẽ được copy lên bộ nhớ RAM để thực thi. Từ đó nó sẽ sinh ra các phân vùng nhớ để lưu trữ các dữ liệu lần lượt bao gồm Text segment, data segment, bss segment, heap segment, stack segment.

## 1. Text Segment
- Đây là vùng nhớ chứa tập hợp các lệnh thực thitry_catch.o
- Trong quá trình chạy vùng nhớ này có quyền đọc và thực thi, và không có quyền ghi vào. Vậy nên các biến ở trong vùng text segment chỉ đọc được giá trị mà không thể thay đổi giá trị.
- Các biến const global hay con trỏ kiểu char global sẽ được ghi vào vùng nhớ này. Còn với biến const, char * local thì dc lưu vào stack segment.
Như ví dụ dưới đây ta có thể thấy địa chỉ của 2 biến nằm cách xa nhau biến const global sẽ được lưu ở vùng text, còn biến const local sẽ được lưu ở vùng stack. Đặc điểm chung là đều không thể thay đổi được biến const, còn khác nhau ở chỗ biến const local sẽ bị thu hồi khi ra khỏi hàm.
```C
#include<stdio.h>
const int a_global = 5; // text
// stack
void test() {
    const int a_local = 10; // stack
    printf("%d\n", &a_local);
}
int main(){
    printf("%d\n", &a_global); 
    test();
}
```
**Kết quả**
```
    4214884 // vung text
    6422268 // vung stack
```

## 2. Data Segment (Initialized Data Segment)
- Đây là vùng dữ liệu đã được khởi tạo, nó sẽ chứa các biến toàn cục khởi tạo giá trị khác 0.
- Chứa các biến Static được khởi tạo khác 0 cả các biến local vẫn global đều được lưu chung vùng nhớ.
- Trong quá trình chạy có thể thay đổi được giá trị, có thể đọc và khi như bình thường. Và tất cả các biến sẽ được thu hồi sau khi kết thúc chương trình
Ở dưới là ví dụ về biến static global và local có thể thấy nó sẽ lưu chung vùng nhớ và 2 biến được khởi tạo khác 0 nên sẽ được lưu ở vùng data. Điểm khác nhau là static local chỉ được sử dụng trong cái hàm chứa biến static local đó.
**Chương trình ví dụ**
```C
#include<stdio.h>
static int static_glo = 6; // data
// stack
void test() {

    static int static_local = 6; // data
    printf("%d\n", &static_local);

}
int main(){
    printf("%d\n", &static_glo);
    test();
}
```
**Kết quả**
```
    4210692 // vùng data
    4210696 // vùng data
```
## 3. Bss Segment (Unitialzed Data Segment)
- Đây là vùng dữ liệu chưa được khởi tạo, nó sẽ chứa các biến được khởi tạo = 0, hoặc chưa được gắn giá trị
- Chứa các biến Static được khởi tạo = 0 hoặc chưa được gán giá trị
- Các biến trong vùng nhớ có thể đọc ghi bình thường. Và khi kết thúc chương trình sẽ thu hồi vùng nhớ.
**Chương trình ví dụ**
```C
#include<stdio.h>
static int static_glo ; // bss
// stack
void test() {

    static int static_local; // bss
    printf("%d\n", &static_local);
}
int main(){
    printf("%d\n", &static_glo);
    test();
}
```
**Kết quả**
```
    4223008 // vùng bss
    4223012 // vùng bss
```

## 4. Stack Segment
- Đây là vùng sẽ chứa các biến local và các tham số truyền vào.
- Có thể đọc ghi thay đổi giá trị như bthg trừ các biến const. Và vùng nhớ sẽ bị thu hồi khi ra khỏi hàm. Ngoại lệ sẽ có biến static local khi nó được lưu ở vùng bss hoặc data.
Như ví dụ bên dưới có thể thấy các biến đều được lưu trữ trong vùng stack dù có khai báo hay không khai báo giá trị miễn là trong hàm thì đều sẽ được lưu ở trong vùng stack
**Chương trình**
```C
#include<stdio.h>
// stack
void test(int c, int d) {
    int e;
    printf("%d\n",&e);
}
int main(){
    int a = 10;
    int b;
    printf("%d\n",&a);
    printf("%d\n",&b);
    test(a,b);
}
```
```
    6422300 \\ stack
    6422296 \\ stack
    6422252 \\ stack
```

## 5. Heap Segment
- Vùng nhớ này là vùng để cấp phát bộ nhớ động trong quá trình chạy chương trình, đây là vùng nhớ cho phép người lập trình tạo ra và giải phóng bộ nhớ theo ý mình muốn. Điều này sẽ giúp cho cta thay đổi tùy ý dựa vào dữ liệu trong quá trình chạy
- Có quyền đọc ghi như bình thường trong quá trình chạy. Ở trong C sử dụng các hàm malloc, calloc, realloc, free để tác động vào vùng nhớ HEAP.

</details>

<details>
<summary><h1>JSON AND LINKED LIST</h1></summary>

## JSON
- Khái niệm: JSON (JavaScript Object Notation - ghi chú về hướng đối tượng JavaScript) đây là 1 kiểu định dạng dữ liệu tuân theo 1 quy tắc nhất định mà hầu hết các ngôn ngữ đều có thể đoc được. JSON là 1 tiêu chuẩn mở để trao đổi dữ liệu giữa máy tính với web, cũng như giữa các hệ thống khác nhau.
- JSON sử dụng cú pháp dựa trên cặp: khóa - giá trị (key : value) để sử dụng dữ liệu. Chuỗi JSON được bao lại bởi dấu ngoặc nhọn {}, các key hoặc value đều phair nằm trong dấu ngoặc kép "key". Giữa các cặp key-value sử dụng dấu phẩy để ngăn
**VD:**
```C
{
    "name" : "Lam",
    "age" : 30,
    "friend" : "[hoa, mai]",
}
```

## LINKED LIST
- Là 1 cấu trúc dữ liệu được sử dụng để tổ chức và lưu trữ dữ liệu. Một linked list bao gồm 1 chuỗi các node. Các node này sẽ lưu trữ dữ liệu và 1 biến con trỏ, trỏ tới địa chỉ tiếp theo (đối với Singlely Linked List), hoặc thêm 1 biến trỏ tới địa chỉ trước đó (đối với Doubly Linked List). Linked list sẽ cung cấp linh hoạt việc thêm, chèn, xóa các phần tử ra khỏi linked list mà không cần phải dichj chuyển toàn bộ các phần tử như trong mảng.
Dưới đây ta sẽ khai báo 1 node cho linked list:
**Ví dụ**
```C
    typedef struct Node{
        int value;
        Node *next;
    }Node; 
```

</details>

<details>

<summary><h1> STACK AND QUEUE </h1></summary>
- Stack là 1 cấu trúc dữ liệu tuân thủ nguyên tắc vào trước ra sau (LIFO) tức là phần tử cuối cùng được thêm vào sẽ là phần tử đầu tiên được lấy ra
+ Các thao tác cơ bản trên stack: push(thêm phần tử vào đỉnh stack), pop (để xóa 1 phần tử ở đỉnh stack), top (lấy giá trị của đỉnh stack)
- Queue là 1 cấu trúc dữ liệu tuân thủ nguyên tắc vào trước ra trước (FIFO) nghĩa là phần tử thêm vào hàng đợi trước sẽ là phần tử đầu tiên được lấy ra
+ Các thao tác cơ bản trên hàng đợi bao gồm: enqueue(thêm phần tử vào cuối hàng đợi), dequeue(lấy phần tử từ đầu hàng đợi), front(để lấy giá trị của phần tử đứng đầu hàng đợi), rear(để lấy giá trị của phần tử đứng cuối hàng đợiđợi)

</details>

<details>
<summary> <h1>CLASS</h1> </summary>
- struct chỉ có member là biến thoi, class member có thể là hàm, có thể là biến, mảng -> class bản chất là 1 file.
- trong 1 class có phạm vi truy cập public, protected, privated.
- tên gọi các member trong class sẽ khác: biến gọi là property, các hàm gọi là method, biến có kiểu dữ liệu là class thì gọi là object.
- Như dưới code dòng không thể khai báo cũng như định nghĩa int SinhVien::test2 = 20 vì biến test2 kia chưa có địa chỉ nên việc gắn vậy compiler sẽ không hiểu để
muốn gắn như ậy phải khởi tạo 1 object (nghĩa là đã khởi tạo 1 địa chỉ).
- Static trong C++: 
+ Với static trong thông thường nó sẽ được khai báo như biến bình thường static int count thì nó sẽ được cấp 1 địa chỉ cố định và tùy thuộc vào
việc biến đó là global hay local thì phạm vi hoạt động nó sẽ khác.
+ Với biến static: Còn với static trong class khi khai báo ở trong class nó vẫn chưa được coi là khai báo nên vẫn chưa được cấp địa chỉ trong bộ nhớ, muốn để nó cấp
địa chỉ cho biến static ta phải khai báo bên ngoài class và được khai báo bên ngoài hàm main vd: int SinhVien::Test = 10; như vậy ta đã cấp được vùng nhớ cho biến 
static và biến static tất cả các object của class sẽ dùng chung địa chỉ với nó. Các hàm trong class thì đều có thể sử dụng biến static.
+ Với hàm static: Điểm đầu tiên 1 function static khác với 1 biến static là nó độc lập với bất kì object nào trong class tức là không cần có object nó cũm có thể
gọi được dữ liệu ra nên không cần 1 object nào thì static function cũm được gọi ra, vậy muốn gọi 1 hàm static ta sẽ chỉ cần gọi trực tiếp với class, ví dụ SinhVien::Name();. Ngoài ra nó có phạm vi bên trong lớp và không thể trỏ bởi object. Và đối với hàm static thì chỉ sử dụng các biến static và hàm static bên trong hàm (tức là những cái property và method bên ngoài hàm, muốn chui vào trong method static thì các cái property và method bên ngoài phải là static, chứ nếu là biến thông thường gọi vào trong method static sẽ bị lỗi). Thì nói về việc các hàm static thì nếu mà gọi ngoài class thì ta mới cần SinhVien::Name(), còn nếu ở trong class ví dụ ta có 1 method khác nằm trong class mà gọi tới Name() này, thì ta không cần phải SinhVien::Name() mà chỉ cần gọi Name() là compiler nó sẽ tự hiểu đó là SinhVien::Name(). Oke về ví dụ phần này ta sẽ nói rõ ở bài 101 của C#

- Các quyền truy cập trong class:
+ public: cho phép object lẫn class con truy cập tới
+ protected: cho phép class truy cập, không cho phép object truy cập
+ private: không cho phép class con, lẫn object truy cập.
** Code class **
```C++
#include <iostream>
#include <string>
using namespace std;

/* ban chat la 1 kieu du lieu */
/* co pham vi truy cap*/
class SinhVien{
    /* member co the la ham, bien, mang .., class ban chat giong nhu 1 file co ham co mang, co the dinh nghia cac kieu du lieu */
    public:
        SinhVien(string ten, string lop);         // constructor
        void display();     // method - phương thức
        static int Test;
    /* nhung member nam trong private thi object khong dung toi dc, no chi co gia tri tai class */
        int test2;
        static void Name(void){
            cout << "This is static function"<<endl;
        }
    private:
        int ID ;             // property - thuộc tính
        string TEN;
        string LOP;
};

int SinhVien::Test = 10;
int SinhVien::test2 = 20;

/*gọi function static*/
void SinhVien::Name();

void SinhVien::display(){     // method
            cout<<"MSV: "<<ID<<endl;
            cout<<"TEN: "<<TEN<<endl;
            cout<<"LOP: "<<LOP<<endl;
}

SinhVien::SinhVien(string ten, string lop){
    ID = 100;
    TEN = ten;
    LOP = lop;
}
int main()
{
    /* string la 1 kieu du lieu thao tac voi chuoi */

    //SinhVien sv; // class - object

    SinhVien sv("Lam","KTMT"), sv1("Dung","VHTT");
    sv.display();

    printf("%d\n",&sv.Test);
    printf("%d\n",&sv1.Test);
}

/* static trong class  */
```

</details>

<details>
<summary><h1> OOP & Virtual </h1></summary>
Trong hướng đối tượng có 4 tính chất tính kế thừa, tính trừu tượng, tính đa hình và tính đóng gói

- **Tính kế thừa(Inheritance):**
.Về cơ bản tính kế thừa tức là việc sử dụng lại các property và method của 1 class lấy từ 1 class khác. Còn được gọi là class con và class cha
.Và các class con chỉ được kế thừa các property và method có quyền truy cập là public và protected của class cha.
.Và có 3 kiểu kế thừa public, protected và private. Các property và method được kế thừa từ class cha sẽ nằm ở quyền truy cập của class con tương ứng với kiểu kế thừa
Không thể kế thừa conductor.
.1 số kiến thức khác : overide(định nghĩa lại) tức là class sẽ ghi đè method của class cha.
.Class con cũm không thể kế thừa constructor của class cha (nếu vẫn thưcj hiện thì bị lỗi compiler).
- Class cha có 3 phạm vi truy cập (public, private, protected)
.Thì class con với kiểu kế thừa public thì các member với phạm vi truy cập ở class cha là public 
thì qua class con vẫn là public, nếu member ở class cha có phạm vi truy cập protected thì sang class con nó sẽ là protected.
.Đối với class con với kiểu kế thừa protected: thì những thằng public và protected từ class cha sang class con sẽ sang protected hết
.Đối với class con với kiểu kế thừa private: cũm tương tự protected thì các member từ class cha sang class đều nằm trong phạm vi truy cập private.
**Ví dụ tính kế thừa**
```C++
    #include <iostream>
    #include <string>   
    using namespce std;

    class DoiTuong{
        private:
            int ID;
            string TEN;
        public:
            void display();
    }

    DoiTuong::DoiTuong(string ten){
        static int id = 100;
        ID = id;
        id++;
        TEN = ten;
    }

    class SinhVien : public DoiTuong{
        
    };
    int main(){

    }
```
- **Virtual Inheritance:**
.Việc sử dụng con trỏ đối với tính kế thừa cũng có sự khác biệt là ta có thể sử dụng con trỏ object của class cha để lấy địa chỉ object của class con còn class con
không lấy được địa chỉ của object class cha. Đối với TH1 ptr->Display() thì chương trình sẽ chạy method Display() của class cha. Và nếu muốn để nó chạy method Display()
của class con thì cta phải sử dụng đến kĩ thuật overload. Tức là kĩ thuật này khi ta trỏ tới cái object của class đó nó sẽ ưu tiên lấy cái định nghĩa của class con.
Như ví dụ bên dưới class cha định nghĩa là void Display(), ở class con cta định nghĩa lại là void Display(string name) nó sẽ định nghĩa đè lên gọi là override. Và khi
cta trỏ tới cái class cha bình thường nó sẽ lấy hàm void Display() được định nghĩa ở class cha nhưng khi ta dùng kĩ thuật overload nó sẽ lấy hàm void Display được
định nghĩa ở class con. Giờ cta muốn nó định nghĩa lại nó sẽ ưu tiên lấy cái method ở class con thì ta sẽ thêm virtual ở trước method.
**VD1 về virtual & pointer in Inheritance:**
```C++
#include <iostream>
#include <string>
using namespace std;
class DoiTuong{
    private:
        int ID;
        string TEN;
    public:
        virtual void Display(){
			cout<<"Ham cha"<<endl;
		}
};
class SinhVien : public DoiTuong{
    public:
        void Display(){
			cout<<"Ham con"<<endl;
		}
};


int main(){
    DoiTuong dt;
    SinhVien sv;

    /*class cha lay dia chi cua class con*/
    DoiTuong *ptr;
    ptr = &sv; 

    ptr -> Display(); // TH1
}
```

- Ở ví dụ dưới này như ta thấy ở class con nó không định nghĩa lại method void display() của class cha mà trong method display() của class nó printf ra word(), nó sẽ nhảy vào thằng cộng cấp với nó và method char *word() cộng cấp với nó nên nó nhảy vào lấy dữ liệu. Và nên nhớ rằng method char *word() ở class con chưa phải là ghi đè mà là định nghĩa lại method. Và muốn ghi đề ta phải sử dụng cái method word() đó ở trong trường hợp cụ thể chứ không thể thông qua method khác để gọi tới nó(TH1 là ví dụ). Và nếu muốn class con khi gọi tới method display và cái method display đó nó ưu tiên gọi method word của class con mà không phải gọi method cộng cấp với nó thì ta sẽ sử dụng virtual. (virtual char *word()).

**VD2 về virtual:**
```C++
#include <iostream>
#include <string>
using namespace std;
class Display{
    public:
        virtual char *word(){
			return (char*)("Class cha\n");
		}
		void f_display(){
			cout<<"Class: "<<word()<<endl;
		}
};
class Child : public Display{
    public:
        char *word(){
            return (char*)"Class Child\n";
        }
};
int main(){
    Display ds;
    Child cd;

	Display *ds1 = &cd;

    ds.f_display();   
    cd.f_display();
	cout<<ds1->word()<<endl;
}

```
- **Tính đóng gói(encapsulation):**
Tính đóng gói là: khi khởi tạo 1 class không được phép truy cập các property trực tiếp mà phải thông qua method để truy cập tới các property. Tức là cta sẽ không để các property ở vùng public vì vùng public cho phép các object truy cập trực tiếp tới, vậy nên ta phải để các property ở vùng protected hoặc ở vùng private, nơi mà object không thể truy cập trực tiếp. Và khi đó ta sẽ sử dụng các method để ở vùng public và object sẽ phải thông qua các method ở vùng public để tác động vào các property. Và tại sao lại bảo vệ các property như vậy? Vì để tránh việc các property bị lấy ra sử dụng không đúng mục đích vì 1 số property chỉ là dữ liệu để nhập vào như các method input và 1 số property sẽ dùng để nhập vào và không dùng để lấy dữ liệu ra. Ví dụ như 1 property được lập trình để chạy tự động tăng dần đến 10 thông qua constructor và cta để nó ở public thì cta có thể từ object truy cập trực tiếp đến dữ liệu của property đó và thay đổi, dẫn đến dữ liệu bị sai sót.

- **Tính trừu tượng(Abstraction)**
.Dựa vào ví dụ bên dưới ta có thể thấy trong 1 hệ thống họ chỉ quan tâm tới việc nhập vào cái gì và lấy ra cái gì chứ không liên quan đến quá trình và mình cũng không cho phép user truy cập đến quá trình để chỉnh sửa, người ta chỉ được phép nhập các method cần nhập như phuongTrinhBacHai() hoặc lấy KQ ra ở method KetQua(). Còn các method thực hiện quá trình tạo ra kết quả như tinhDelta phải ẩn trong private hoặc protected.
.Sự khác nhau giữa tính đóng gói và tính trừu tượng: Tính đóng gói thì các property không được phép truy cập trực tiếp mà phải thông qua method, còn tính trừu tượng thì những cái quá trình để giải ra kết quả cuối cùng sẽ bị ẩn đi. Mặc dù cùng nằm ở privated or protected nhưng ý nghĩa nó khác nhau.
**VD Abstraction:**
```C++
#include <iostream>
class phuongTrinhBacHai{
    private:
        int x1;
        int x2;
        float tinhDelta(int a, int b, int c);
        double KETQUA;
    public:
        phuongTrinhBacHai(int a, int b, int c){
            tinhDelta(a,b,c) >= 0;

            ....
        }
        double KetQua(){
            return KETQUA;
        }
};

```
- **Tính đa hình(Polymorphism):**
.Thì trong C++ cho phép các hàm có thể cùng tên với nhau nhưng khác parameter hoặc khác kiểu dữ liệu trả về.
.Tính đa hình là 1 input có nhiều parameter khác
**VD về polymorphism:**
```C++
#include <iostream>
#include <string>
class TinhToan(){
    private:
        int a;
        int b;
    public:
        void Tong(int a, int b){
            printf("Tong: %d\n",a+b);
        }
        int Tong(int a, int b, int c){
            return a+b+c;
        }
        double Tong(int a, double b){
            return (double)a+b;
        }
};

int main(){

}
```
</details>
<details>

<summary><h1> STL </h1> </summary>
STL (Standard Template Library) là 1 thư viện trong lập trình C++ cung cấp 1 tập hợp các template class và functions để thực hiện nhiều loại cấu trúc dữ liệu và thuật 
toán phổ biến. Một số thành phần chính của STL:

- **Interator theory:** cung cấp 1 cách chung để duyệt qua các phần tử của 1 container mà không cần biết chi tiết về cách container đó triển khai. Có thể nói Interator là 1 đối đối tượng cho phép truy cập tuần tự qua các phần tử của 1 container và nó giống như con trỏ, cho phép di chuyển qua các phần tử trong container (tức là 1 object của iterator được khai báo ra sẽ là 1 con trỏ). Và tại sao nó lại làm được vậy? Vì pointer sẽ nắm được địa chỉ của dữ liệu của container và cta chỉ cẩn sử dụng
con trỏ đó để truy cập vào từng thành phần dữ liệu bằng việc sử dụng toán tử ++. VD như cta sử dụng vector hay list thì cũm chỉ cần ++ là sang được phần tử tiếp theo để duyệt. Tuy nhiên không phải tất cả các iterator có chức năng tương tự như con trỏ. Và có 5 loại Iterator và mỗi loại iterator sẽ support cho từng container khác nhau và có 1 số container không cần sử dụng Iterator(Stack, Queue, Priority-Queue):
.Random-Acess: support for -> Vector, Deque
Đây là iterator mạnh nhất vì nó không bị giới hạn về chức năng, nó sẽ gồm tất cả các tính chất của các iterator bên dưới và ngoài ra điều đặc biệt của iterator này là có thể truy cập bất cứ phần tử nào mà không cần duyệt lần lượt container điển hình ở đây chính là vector, giống như mảng vậy nó chỉ cần biết có bao nhiêu phần tử và nhảy được tới phần tử bất kỳ để lấy giá trị hoặc làm bất cứ gì mình muốn sử dụng gì từ nó


.Bidirectional: support for -> List, Map, Multimap, Set, Multiset.
Iterator chức năng của nó cũng để duyệt các phần tử trong container, như ta biết forward iterator nó chỉ duyệt được 1 hướng thì đối với bidrectional nó cũm có các tính chất tương tự với forward iterator nhưng khác ở chỗ nó có thể duyệt được 2 hướng tức là nó đang duyệt ++ lên thì nó có thể -- về khác với forward iterator nó chỉ có thể tiến hoặc lùi.
```C++
#include <iostream>
#include <vector>
using namespace std;
int main(){
    vector<int> v1 =  {1,2,3,4,5};
    vector<int> :: iterator i1;

    // can see that bidirectional can ++ and -- 
    for(i1 = v1.begin(); i1 != v1.end();){
        if(*i1%2 == 0){
            i1++;
        }
        if(*i1%2 != 0){
            *i1 = 2;
            i1--;
        }
    }

}
```





.Forward iterator: Iterator này sẽ cao hơn so với 2 Iterator input và output, nó là sự kết hợp của 2 Iterator, nó cho phép cả việc gán phần tử, lẫn truy cập phần tử. Ngoài ra nó có thể duyệt nhiều lần trong 1 lần khai báo iterator, tức là ngaoif viêcj nó kết hợp cả 2 tính chất của Input và Output Iterator là vừa truy cập để đọc và gắn vào phần tử thì nó còn tái sử dụng nhiều lần trong 1 chương trình thay vì 1 lần như Input và output iterator. Mà thêm nữa cả Input, Output và Forward Iterator đều là dịch chuyển theo 1 hướng và tuần tự tức là nó sẽ duyệt lần lượt từng phần tử 1 và không thể quay đầu lại cũng như không thể nhảy tới ngẫu nhiên các phần tử khác trong container.
```C++
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v1 = {1,2,3,4,5};
    vector<int> :: iterator i1;
    i1 = v1.begin();
    while(i1 != v1.end()){
        if(*i1 != new_value) {*i1 = new_value;} // forward iterator - can be assigned and access elements
        i1++;
    }
}

```


.Output Iterators: Giống như Input Iterator có chức năng rất hạn chế, chỉ được sử dụng trong các thuật toán 1 lần, khác cái nó không phải để truy cập phần tử mà để gán phần tử.
VD về output iterator:
```C++
// tinh chat cua output iterator tuc la cho phep gan 
// lvalue la truy cap toi 1 cai dia chi de thay doi gia tri rvalue la lay du lieu cua 1 dia chi 

// C++ program to demonstrate output iterator
#include<iostream>
#include<vector>
using namespace std;
int main()
{
    vector<int> v1 = {1,2,3,4,5};

    //Declaring an iterator
    vector<int> :: iterator i1;

    // It means i1 similar pointer and it take first address of vector and it pass through elements of vector with output iterator just assign value for elements whereas input iterator just access memory of elements and takes value of them.

    // And output iterator it just do it 1 time during 
    for(i1 = v1.begin(); i1 != v1.end(); ++i1){
        *i1 = 1;
    }
    return 0;
}


```

.Input Iterators: Đây là Iterator thấp nhất trong tất cả các Iterator và nó bị giới hạn về chức năng, nó chỉ được sử dụng trong các thuật toán 1 lần, tức là các thuật toán xử lý container theo trình tự, sao cho không có phần tử nào được truy cập nhiều hơn 1 lần. Nói rõ hơn về cơ chế hoạt động là khi 1 iterator được khai báo và cái iterator đó chỉ được sử dụng đúng 1 lần để duyệt 1 container và không thể tái sử dụng iterator đó nữa.  
VD về input iterator:
```C++
// tinh chat cua input iterator tuc la truy cap va lay gia tri
// lvalue la truy cap toi 1 cai dia chi de thay doi gia tri rvalue la lay du lieu cua 1 dia chi 

// C++ program to demonstrate input iterator
#include <isotream>
#include <vector>

using namespace std;

int main(){
    vector<int> v1 = {1,2,3,4,5};

    vector<int> :: iterator i1;
    for(i1 = v1.begin(); i1 != v1.end(); ++i1){
        cout << (*i1) << endl;
    }
    return 0;
}

```


- **Container:** Một container là 1 cấu trúc dữ liệu chứa nhiều phần tử theo 1 cách cụ thể, có thể hiểu nó quản lí 1 không gian lưu trữ các property và tạo ra các method
để cho người dùng sử dụng để truy cập tới chúng hoặc là truy cập trực tiếp thông qua iterators. Và dưới đây là 1 số container phổ biến
**Code Vector+Map+algorithm**
```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

void f_Nhap(vector<int> *v1);
void f_In(const vector<int> &v1);
void f_Tong(const vector<int> &v1);
void f_max(const vector<int>&v1);
void f_min(const vector<int>&v1);
void f_remove_even(vector<int>*v1);
void f_TangDan(vector<int>*v1);
void f_GiamDan(vector<int>*v1);
void f_CheckMaxNumber(const vector<int>&v1);

int main(){
	int number;
	cout<<"Nhap so: ";cin>>number;

	vector<int> v1(number);
	f_Nhap(&v1);
	f_In(v1);
	f_Tong(v1);
	f_max(v1);
	f_min(v1);
	f_TangDan(&v1);
	f_In(v1);
	f_CheckMaxNumber(v1);
}

void f_Nhap(vector<int> *v1){

	for (int i = 0; i < (*v1).size(); i++)
	{
		cout<<"Nhap v"<<i<<": ";
		cin>>(*v1)[i];
	}
}
void f_In(const vector<int> &v1){
	cout << "Output: ";
	for (int i = 0; i < v1.size(); i++)
	{
		cout<<v1[i]<<" ";
	}
	cout<<endl;
}
void f_Tong(const vector<int> &v1){
	int i_Tong = 0;
	for (int i = 0; i < v1.size(); i++)
	{
		i_Tong += v1[i];
	}

	cout<<"Tong: "<< i_Tong<<endl; 
}
void f_max(const vector<int>&v1){
	int i_max = *max_element(v1.begin(), v1.end());
	cout <<"Max: "<<i_max<<endl;
}
void f_min(const vector<int>&v1){
	int i_min = *min_element(v1.begin(), v1.end());
	cout <<"Min: "<<i_min<<endl;
}
void f_remove_even(vector<int>*v1){
	for (int i = v1->size()-1; i >=0; i--)
	{
		if(((*v1)[i])%2 == 0){
			v1->erase(v1->begin() + i);
		}
	}
}

void f_GiamDan(vector<int>*v1){
	cout<<"Sort giam dan"<<endl;
	sort(v1->begin(),v1->end(), [](int a, int b){return a>b;});
}
void f_TangDan(vector<int>*v1){
	cout<<"Sort tang dan"<<endl;
	sort(v1->begin(),v1->end(), [](int a, int b){return a<b;});
}

void f_CheckMaxNumber(const vector<int>&v1){
	map<int, int> m_check;
	
	for(auto &num : v1){
		m_check[num]++;
	}
	int max_count=0;
	int element=0;
	for(auto &p : m_check){
		if(p.second > max_count){
			max_count= p.second;
			element = p.first;
		}
	}
	cout<<"Phan tu: "<<element<<" xuat hien "<<max_count<<endl;
}
```

**Code List**
```C++
#include <iostream>
#include <list>
#include <algorithm>
#include <map>
using namespace std;

void f_Nhap(list<int> *l1, int n);
void f_In(const list<int> &l1);
void f_Max(const list<int> &l1);
void f_Min(const list<int> &l1);
void f_revrse(list<int> *l1);
void f_erase(list<int> *l1);
void f_insert(list<int> *l1);
void f_checkList(const list<int> &l1, int size);
void f_mergeList(const list<int> &l1, const list<int> &l2);
void f_Mul_erase(list<int> *l1);

int main(){
	list<int> l1;
	int n;
	cout<<"Nhap so phan tu: "; cin>>n;
	f_Nhap(&l1, n);
	f_Mul_erase(&l1);
	f_In(l1);
	/*f_Max(l1);
	f_Min(l1);
	f_revrse(&l1);
	f_In(l1);
	f_erase(&l1);
	f_In(l1);
	f_insert(&l1);
	f_insert(&l1);
	f_insert(&l1);
	f_In(l1);
	cout << l1.size()<<endl;
	f_checkList(l1,l1.size());
	
	list<int> l2;
	f_Nhap(&l2,l1.size());

	f_mergeList(l1,l2);*/

}

void f_Nhap(list<int> *l1, int n){
	for (int i = 0; i < n; i++)
	{
		int x;
		cout<<"Nhap x"<<i<<": ";cin >> x;
		(*l1).push_back(x);
	}
}
void f_In(const list<int> &l1){
	for(auto h : l1){
		cout<< h << " ";
	}
	cout<<endl;
}
void f_Max(const list<int> &l1){
	int i_max = *max_element(l1.begin(),l1.end());
	cout << "Max= "<<i_max<<endl;
}
void f_Min(const list<int> &l1){
	int i_min = *min_element(l1.begin(),l1.end());
	cout<<"Min = "<<i_min<<endl;
}

void f_revrse(list<int> *l1){
	l1->reverse();
}

void f_erase(list<int> *l1){
	int x;
	cout<<"So can xoa: ";cin>>x;
	list<int>::iterator it;
	for ( it = l1->begin(); it != l1->end();)
	{
		if((*it)==x){
			it = l1->erase(it);
		}
		else{
			it++;
		}
	}
}

void f_insert(list<int> *l1){
	int x;
	cout << "Phan tu can them: "; cin>> x;
	list<int> ::iterator it;
	if(l1->size()==0){
			l1->push_back(x);
	}
	else{
		for (it = l1->begin();  it != l1->end();)
		{
			if (*it <=  x)
			{
				it++;
			}
			else{
				l1->insert(it,x);
				break;			
			}
			
		}
		if(it == l1->end()){
			l1->push_back(x);
		}
	}
}

void f_checkList(const list<int> &l1, int size){
	list<int> l2;
	int so_l2;
	int count = 0;
	list<int> :: const_iterator it1;
	list<int> :: iterator it2;
	for ( int i = 0; i<size; i++){
		cout<<"Nhap gia tri cua l2: ";cin>>so_l2;
		l2.push_back(so_l2);

	}

	for (it1 = l1.begin(); it1 != l1.end(); it1++)
	{
		for (it2 = l2.begin(); it2 != l2.end(); it2++)
		{
			if(*it1 == *it2){
				count++;
			}
		}
	}

	if(count == size){
		cout << "2 list giong nhau"<<endl;
	}
	else{
		cout << "2 list khac nhau"<<endl;
	}
	
}
void f_mergeList(const list<int> &l1, const list<int> &l2){
	list<int> l3;
	l3.insert(l3.end(),l1.begin(),l1.end());
	l3.insert(l3.end(),l2.begin(),l2.end());
	
	l3.sort();
	f_In(l3);
	l3.sort([](int a, int b){return a>b;});
	f_In(l3);
}


void f_Mul_erase(list<int> *l1){
	map<int,int> list_map;
	list<int> ::iterator it;
	for(it = l1->begin(); it != l1->end();){
		list_map[*it]++;
		if (list_map[*it]>=2)
		{
			it = l1->erase(it);
		}
		else{
			it++;
		}
	}
}
```
**Map:**
```C++
#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

void f_Nhap(auto *n_map, int n);
void f_countNumber(const map<int,int> &i_map);
void f_countChar(map<char,int> *c_map);
void f_checkWord();
int main(){
	/*map<int,int> i_map;
	int n;
	cout<<"So phan tu: ";cin>>n;
	f_Nhap(&i_map, n);
	f_countNumber(i_map);
	
	map<char,int> c_map;
	f_countChar(&c_map);*/

	f_checkWord();

}

void f_Nhap(auto *n_map, int n){
	int nhap;
	for (int i = 0; i < n; i++)
	{
		cout<<"key: ";cin>>nhap;
		(*n_map)[nhap]++;
	}
}
void f_countNumber(const map<int,int> &i_map){
	cout << "Tan suat xuat hien"<<endl;
	for(auto p : i_map){
		cout << p.first <<": "<<p.second<<endl;
	}
}
void f_countChar(map<char,int> *c_map){
	string s;
	cout<<"Nhap chu: ";cin>>s;
	for(auto p: s){
		(*c_map)[p]++;
	}
	for(auto p : *c_map){
		cout << p.first <<": "<<p.second<<endl;
	}
}
void f_checkWord(){
	string s;
	getline(cin,s);
	transform(s.begin(),s.end(),s.begin(),::tolower);
	
	stringstream  ss(s);
	string word;
	map<string, int> countWord;

	while (ss>>word)
	{
		countWord[word]++;
	}
	for(auto p: countWord){
		cout<< p.first << ": "<<p.second<<endl;
	}
	
}
```
**Set:**
```C++
#include <iostream>
#include <set>

using namespace std;

int main(){

	set<int> s;
	int n;
	int x;
	cout<<"Nhap so phan tu: ";cin>>n;

	/* Nhap */
	for (int i = 0; i < n; i++)
	{
		cout<<"Nhap: ";cin>>x;
		s.insert(x);
	}
	for(int p: s){
		cout << p <<" ";
	}

	/* check so luong phan tu */
	cout<<endl;
	cout<<"So luong phan tu khac nhau: "<<s.size()<<endl;
	
	/* tim kiem phan tu */
	int find;
	cout<<"Phan tu can tim: ";cin>>find;
	auto it = s.find(find);
	if(*it == find){
		cout<<"Tim thay phan tu can tim la: "<<*it<<endl;
	}
	else{
		cout<<"Khong tim thay"<<endl;
	}
	
	/*In ra nguoc day */
	cout<<"In nguoc: ";
	for(auto it = s.rbegin(); it != s.rend();it++){
		cout << *it <<" ";
	}
	cout<<endl;


}
```

</details>


<details>
<summary><h1> Template </h1></summary>

-  template được sinh ra ví dụ như 1 chương trình cần có 2 hàm tính tổng chẳng hạn int Tong(int a, int b) và float Tong(float a, float  b) thì nếu như không có template phải tạo ra 2 method nhưng nếu dùng template chỉ cần tạo ra 1 methoad còn kiểu dữ liệu của nó như nào do mình nhập vào.

- **Syntax:**
+ template function: như ví dụ bên dưới thì các biến có kiểu dữ liệu var do template định nghĩa thì nó sẽ dựa vào cái kiểu dữ liệu truyền vào như nào mà kiểu dữ liệu của các var cũng thay đổi tương tự.
```C++

    /*template function*/
    template<typename var, typename var1> // template<class var> cum dc
    var Tong(var a, var1 b){
        return (var)(a+b);
    }

    int main(){
        // duoc khai bao Tong(3,8) boi vi ham duoc goi truc tiep ma khong can object nao nen khong can khoi tap cong kenh nhu class
        printf("Tong 2 so %d", Tong(3,8.5));
    }

```
+ template class: 
```C++
    /*template class*/
    template <typename type> 
    class Sensor
    {
        private:
            type data;
        public:
            SensorData(type initData) : data(initData){}
            void updateData(type newData){
                data = newData;
            }
            type getData const(){
                return data;
            }
    }

    int main()
    {
        /*khoi tao template class*/
        SensorData<double> temperatorSensor(36.5);
    }
```

</details>

<details>
<summary><h1> Namespace </h1></summary>

    - Namespace là 1 cơ chế trong lập trình giúp tổ chức các biến các hàm, các lớp. Như ta biết chương trình sẽ có nhiều người viết thì nhiều lúc sẽ xảy ra việc chọn các cái hàm và các biến có tên giống nhau. Thì việc sử dụng namespace nó sẽ quản lý riêng các cái tên ấy, tức là cái tên đó nó sẽ thuộc 1 cái namespace khác nhau
    VD: biến a thuộc 2 cái namespace khác nhau thì không làm sao
    - Đặc điểm của namespace thì nó có thể có 1 cái namespace khác trong nó nữa, và nó có thể sử dụng using namespace ... để thay thế cái tên nó định nghĩa như using namespace std chẳng hạn. Cái namespace có thể mở rộng ở bất cứ file nào -> ví dụ là chúng ta vẫn có thể namespace std { int a; } và sử dụng std::a, tức là có thể mở rộng nó thỏa mái mà không cần phải vào cái namespace gốc

</details>

<details>
<summary><h1> Lambda </h1></summary>

- Nó là 1 cái hàm cục bộ / hàm vô danh nó có thể được khai báo và sử dụng ngay tại chỗ. Ứng dụng nó hay dùng là ở trong STL đặc biệt với các std::for_each và std::sort đây là những stl có tham sô đầu vào là 1 con trỏ hàm -> lambda hợp vì nó sử dụng xong sẽ giải phóng luôn, còn nếu hàm bthg sẽ vẫn ở trên bộ nhớ.
- Lambda khác với hàm thông thường điểm gì? Thì nó sẽ khác ở vùng nhớ gọi, cách thức gọi như hàm bình thường thì nó sẽ dùng function call, còn với lambda nó sẽ giống 1 inline function. Ngoài ra lambda còn có khả năng nắm giữ trạng thái thông qua từ khóa mutable và lambda có thể truyền như 1 biến hoặc nó cũm không cần tên, gọi là chạy.

- **Syntax:**
    [capture] ( parameter )-> return type{}
    + capture là nơi lambda lấy các biến từ bên ngoài vào by value[=], by reference[&] tức là không cho con trỏ, tức là biến bên trên là biến thường hay biến con trỏ đều truyền vào được hết. Nhưng mà nó không được truyền vào dưới dạng con trỏ tức là *a, *b ... chỉ được a,b;
    + parameter là tham số truyền vào cho lambda và khi thoát khỏi hàm nó sẽ mất thoi, như 1 hàm thông thường

    Một số chú ý thông qua ví dụ bên dưới thì sayHello kia sẽ chứa địa chỉ của hàm Lambda.
    **VD về việc khai báo lambda:**

    ```C++
        int main(){
            // khai báo cơ bản
            auto sayHello = [](){
                cout<< "Hello World"<< endl;
            }
            sayHello();

            // khai báo đầy đủ
            int x;
            auto Tong = [x](int a, int b)->int{
                return a+b;
            }
            
        }

    ```
    + Nói thêm về mutable trong lambda tức là khi cta cho 1 cái giá trị vào hàm thì khi thoát khỏi hàm nó sẽ bị giải phóng, thì lambda cũm vậy, nên nó dùng từ khóa mutable để được phép thay đổi giá trị của biến truyền vào.
    **VD:**
    ```C++
        int main(){
            int count = 0;
            auto increament = [count]() mutable ->int{
                return ++count;
            }
        }
        increament(); // count = 1
        increament(); // count = 2
        /*tức là truyền biến tham trị nhưng vẫn không bị triệt tiêu giá trị mà nhờ từ khóa mutable mà nó sẽ thay đổi được giá trị*/
    ```

</details>


<details>
<summary><h1> Smart Pointer </h1></summary>

**Unique Pointer**
2. Shared Pointer
3. Weak Pointer

</details>

<details>
<summary><h1> Design Pattern </h1></summary>



</details>

<details>
<summary><h1> Thread </h1></summary>

Thì như ta thấy thread nó là 1 cái class ta khai báo như bình thường thread t1(func1) như vậy chẳng hạn. Ở đây ta cần chú ý cái lớn nhất là khi ta khai báo 1 cái thread nó đã bắt đầu vào luồng ròi. Tức là chúng ta không cần phải .join() thì nó vẫn chạy như bình thường, tức là 3 luồng vẫn chạy. Vậy .join() ở đây sẽ có tác dụng gì? .join() ở đây có tác dụng chặn luồng đấy kết thúc nếu nó chưa chạy xong. Tức là ví dụ trong main ta chỉ gọi 2 cái thread kia và không làm gì trong main cả và khi cta run nó sẽ lỗi "terminate called without an active exception" lí do nó lỗi là 2 task kia vẫn chưa được chạy xong thì chương trình đã kết thúc hoặc đơn giản là chúng ta không gọi .join(). Vậy tại sao không gọi .join() lại bị lỗi :))) vì bản chất thread là 1 class và nó sẽ có hàm destructor và trong cái hàm destructor này nó sẽ đọc .joinable() nếu .joinable() trả về flase (tức là chúng ta đã gọi .join() cho đối tượng thread) thì sẽ không bị lỗi. Và ngược lại trả về true là sẽ hiện ra lỗi "terminate called without an active exception". 
```C++
int main(){
    std::thread task1(func1);
    std::thread task2(func2);   
}

Kết quả: terminate called without an active exception
```

**Code nhập môn thread :))**
```C++
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void delays(int time){
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

void func1(){
    for (int i = 0; i < 4; i++)
    {
        std::cout<<"i func1: "<<i<<std::endl;
        delays(1);
    }
}
void func2(){
    for (int i = 0; i < 10; i++)
    {
        std::cout<<"i func2: "<<i<<std::endl;
        delays(1);
    }
    
}

int main(){
    std::thread task1(func1);
    std::thread task2(func2);

    for (int i = 0; i < 18; i++)
    {
        cout<< i<<endl;
        delays(1);
    }
    
    task1.join();
    task2.join();
    
}

```
Tiếp theo ta sẽ nói về Atomic. Thì ở ví dụ dưới đây ta có 2 thread chạy cùng 1 function và cũm như nó share chung 1 biến để tác động vào là biến sharedVar. Việc share chung 1 biến như vậy có thể khiến giá trị của cái biến đấy nhiều lúc hoạt động không chính xác. Vì ở đây ta có 2 thread chạy // tác động vào biến đó, thì như t được biết để thay đổi 1 biến hay ví dụ ở đây là để ++ 1 biến, ta phải trải qua 3 bước assembly, thứ nhất đó là đọc giá trị của biến từ ram vào register, thứ 2 là thay đổi giá trị biến trên register, thứ 3 sẽ là cập nhật giá trị mới từ register vào đúng địa chỉ của sharedvar. Thì với việc 2 thread đều truy cập vào 1 biến nhiều lúc thread thứ 1 đang thực hiện thao tác với biến ở bước đọc vào giá trị từ ram vào register (ví dụ giá trị là 100) và chưa kịp làm gì hết các bước đã nhảy sang thread 2 và lúc này thread 2 cũm sẽ đọc từ ram và giá trị lúc này vẫn là 100, vì thread 1 chưa kịp lưu lại. Thì sau khi thread 2 xong giá trị trong RAM lúc này là 101 và quay về thread 1 đáng lẽ nó phải lên 102 nhưng mà do nó phải thực hiện tiếp thao tác còn đang dở, tức là vừa nó lưu là 100 giờ nó ++lên sẽ là 101 -> nó lại lưu vào là 101. Có nghĩa là qua 2 luồng thread giá trị chỉ tăng 1 thay vì tăng 2 -> lỗi. Và để khắc phục điều đó ta có atomic, nó sẽ bắt mình phải chạy hết các bước assembly

**Kỹ thuật đồng bộ hóa luồng**

Atomic là 1 kỹ thuật động bộ hóa các luồng, cho phép 1 luồng thực thi toàn bộ thao tác mà không bị ảnh hưởng bởi luồng khác. Nó thực chất là 1 cái struct template tạo ra các cơ chế các hàm để khi mình thao tác 1 biến, hàm nào đó, nó sẽ có cơ chế bảo vệ không cho các thao tác bị gián đoạn. Nếu mở hẳn thư viện của atomic ấy mình sẽ thấy khá nhiều operator cho mỗi loại varible như atomic<int> varInt(0) -> có thể varInt++ được vì có operator đó, còn atomic<double> varDouble(0) không thể varDouble++ được vì không có operator đó trong class atomic

Mutex thì nó cũm là kĩ thuật tương đối giống Atomic nhưng thay vì chỉ sử dụng trong 1 biến, nó sử dụng được trong 1 đoạn code, nó nhằm giới hạn quyền truy cập của các luồng. Ví dụ luồng A đang truy cập ròi thì luồng B không được truy cập đoạn code đấy 
**Code Atomic and Mutex**
```C++
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
using namespace std;

//int sharedVar = 0;

atomic<int> sharedVar(0);
mutex mtx;

void Task_Inc(int iter){
    for(int i = 0; i<iter;i++){
        mtx.lock();
        cout << ++sharedVar << endl;
        mtx.unlock();
    }
}

int main(){
    thread t1(Task_Inc,15000);
    thread t2(Task_Inc,15000);

    t1.join();
    t2.join();

    cout << "Final result: "<< sharedVar <<endl;

}
```
**Lock Guard, Unique Lock**
Lock Guard và Unique Lock nó là 1 phần của thằng Mutex tức là nó là 1 cơ chế nằm trong Mutex

- Lock Guard là cơ chế tự động khóa và tự động mở khóa tức là trong cái template class của lock guard này nó sẽ có constructor gọi ra mtx.lock() và destructor sẽ gọi ra mtx.unlock() -> tự gọi khi khởi tạo object và tự hủy khi object kết thúc -> nó phải là biến local để tự kết thúc. Như trong code dưới là thoát khỏi while cái là nó sẽ destructor cái biến đấy

```C++
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>

using namespace std;

mutex mtx;
int counter = 0;

void delay(uint32_t time){
    this_thread::sleep_for(chrono::seconds(time));
}

void display(int id)
{
    while(1){
        lock_guard<mutex> lock(mtx);
        cout<< "this is task display: "<<id<<endl;
        cout<< "counter: "<<++counter << endl;
        delay(1);
    }
}

int main(){
    thread t1(display, 1);
    thread t2(display, 2);
    thread t3(display, 3);

    t1.join();
    t2.join();
    t3.join();
}

```

- Unique Lock nó khá giống Lock Guard nó khác chỗ nó linh hoạt hơn Lock Guard, Lock Guard sẽ khóa và mở khóa tự động, còn unique mở khóa và khóa thủ động nữa. Tức là unique lock sẽ có các chế độ .lock() và .unlock() khá giống với mutex thường. Nhưng khác chỗ là trong unique lock những cái hàm lock này là không phải những cái method có sẵn của mutex mà nó tự phát triển ra. Nó sẽ có cả chế độ tự động và thủ công thay vì thủ công như mutex thường. Vậy sao có unique lock ròi vẫn có lock guard thì có thể hiểu đơn giản là để code nó clean thoi, kiểu khi nhìn thấy lock_guard ta sẽ biết đây là 1 hàm đơn giản, còn với unique lock thì maybe nó sẽ phức tạp.

```C
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>

using namespace std;

mutex mtx;
int counter = 0;

void delay(uint32_t time){
    this_thread::sleep_for(chrono::seconds(time));
}

// tự động tương tự lock_guard
void display(int id)
{
    while(1){
        unique_lock<mutex> lock(mtx);
        cout<< "this is task display: "<<id<<endl;
        cout<< "counter: "<<++counter << endl;
        delay(1);
    }
}

// thủ công
void task(int id){
    while (1)
    {
        unique_lock<mutex> ulock(mtx);
        cout << "Peripheral HMI " << id << endl;
        ulock.unlock();
        delay(1);

        ulock.lock();
        cout << "Peripheral camera "<<id<<endl;
        ulock.unlock();
        delay(2);

        ulock.lock();
        cout<< "Peripheral keyboard " << id <<endl;
        ulock.unlock();
        delay(3);
    }
}

int main(){
    thread t1(task, 1);
    thread t2(task, 2);
    thread t3(task, 3);

    t1.join();
    t2.join();
    t3.join();
}

```

**Condition Variable**

Thì như ta được biết những cái thread trước của cta nó sẽ không có thứ tự vào các thread, như kiểu nhiều lúc vào thread 1 trước, hoặc vào thread 2 trước hoặc vào thread 3 trước, đó là thứ ta không kiểm soát được. Thì nó sinh ra cái condition variable để tạo ra thứ tự nhất định giữa các thread. Ví dụ như 1 thread đọc data trước và 1 thread in ra data và cái thread in ra data này sẽ phụ thuộc vào thread đọc data, khi nó đọc dữ liệu ròi thì mới có dữ liệu in ra. Thì đấy là ví dụ như thế nó nằm ở ví dụ của phần Producer - Consumer Module bên dưới.

Oke giờ nói về cái wait đi thì cái hàm cv.wait kia giữ cho thread đó ở trạng thái ngủ, tức là chờ điều kiện nào đóa xảy ra thì nó sẽ thức giấc. Như trong ví dụ 1 nó chờ bên read data đọc được dữ liệu và gửi 1 cái notify thì cái notify này nó là điều kiện để cv.wait thức giấc. Và khi thức giấc nó sẽ check điều kiện bên cạnh nó là true hay false. Còn nếu không thức thì nó không thèm check luôn :)) . Và khi nó thức ròi nếu mà thấy flase lại ngủ tiếp còn true thì chạy xuống bên dưới. 

**Ví dụ 1**
```C++
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
using namespace std;
mutex mtx;
atomic<int> data_sensor(0);
bool check_data = false;
condition_variable cv;

void delay(int time){
    this_thread::sleep_for(chrono::seconds(time));
}

void read_data(){
    while(1){
        data_sensor++;
        check_data = true;
        unique_lock<mutex> ulock(mtx);
        cout<<"Read data successful"<<endl;
        ulock.unlock();
        cv.notify_one();
        delay(1);
    }
}

void process_data(){
    while(1){
        unique_lock<mutex> ulock(mtx);
        cv.wait(ulock, []()->bool{return check_data;});
        cout<<"Data = "<<data_sensor<<endl;
        check_data = false;
        ulock.unlock();
        delay(1);
    }
}
int main(){
    
    thread t1(read_data);
    thread t2(process_data);   
    t1.join();
    t2.join();
}

```

**Producer - Consumer Model**
Như cái tên đây là 1 mô hình cho việc sản xuất và tiêu thụ, nó dùng để quản lý các quá trình, chính xác hơn là quản lý việc chia sẻ dữ liệu giữa các tiến trình,nó cũm giống như Condition Variable có thứ tự thực hiện. Như trong mô hình này thì producer có vai trò tạo ra data và cho chúng vào buffer, còn consumer sẽ là bên tiêu thụ dữ liệu lấy từ buffer đó. Và nếu như producer mà trống thì consumer sẽ chờ cho producer tạo dữ liệu. Còn nếu producer mà đầy (buffer) thì chờ cho consumer tiêu thụ bớt ròi mới đẩy dữ liệu vào.

Và như chương trình code dưới đây, ta thấy có 2 cv.notify_one(), cũm như có 2 cv.wait() vậy, thì khi nó notify nó sẽ tìm tới wait nào ?? thì như ta tìm hiểu, nó sẽ có cơ chế tìm đến wait() chưa được đánh thức, nếu cả 2 chưa được đánh thức nó sẽ chọn ngẫu nhiên 1 wait(). Điều này dẫn đến như nào :)) thì có thể nó sẽ luôn đánh thức 1 wait() thoi nếu như cả 2 wait() không wait nào chịu dậy. Còn nếu 1 wait() đã thức thì nó sẽ notify() cho wait() còn lại.

```C++

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <vector>

using namespace std;

mutex mtx;
condition_variable cv;
atomic<int> count_data(rand());

vector<int> buffer;

void delay(int time){
    this_thread::sleep_for(chrono::seconds(time));
}

void producer_data(){
    while(1){
        unique_lock<mutex> unilock(mtx);
        cv.wait(unilock, []()->bool{return buffer.size()<5;});
        buffer.push_back(count_data);
        cout<< "Add data: " <<count_data<<endl;
        count_data = rand()%100;
        unilock.unlock();
        cv.notify_one();
        delay(1);
    }
}

void consumer_data(){
    while(1){
        
        unique_lock<mutex> unilock(mtx);
        cv.wait(unilock, []()->bool{return !buffer.empty();});
        cout << "Process data: " << buffer.front()<<endl; 
        buffer.erase(buffer.begin());
        unilock.unlock();
        cv.notify_one();
        delay(5);
    }
}


int main(){
    thread t1(producer_data);
    thread t2(consumer_data);

    t1.join();
    t2.join();
}

```

**Sự khác biệt join() và detach()**

join() sẽ giống như là 1 cái hàm của chương trình main(), kết thúc thread.join() thì chương trình bên dưới main() mới chạy tiếp, nghĩa là thread.join() sẽ luôn hoàn thành khi chương trình main() kết thúc, còn nếu mà join() mà chưa hoàn thành mà chương trình main() đã kết thúc nó sẽ sinh ra lỗi.
Còn đối với detach() nó sẽ chạy song song với chương trình main(), tức là các thread cứ làm việc của thread, còn main() sẽ làm việc của main(), nhưng khi main() kết thúc các chương trình thread cũm bị kết thúc theo.
Đặc biệt khi dùng detach() ta không thể quản lý được nó nữa, nghĩa là bình thường ta sẽ khai báo 1 object để chạy thread.detach(), nhưng ngay sau khi ta detach() xong là object đấy sẽ bị hủy luônn, không thể dùng lại object đó để quản lý như lấy thông tin joinable() hay .join() lại. Còn đối với join() ta vẫn có thể joinable() hoặc lấy kết quả sẽ dễ dàng hơn.

Test với chương trình này thì thấy dù t1.detach() hat t2 vẫn chưa chạy xong khi kết thúc chương trình nó không bị lỗi, chứ như .join() là lỗi ròi

```C++
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

atomic<int> count(0);

void delay(int time){
    this_thread::sleep_for(chrono::seconds(time));
}

void task1(int id){
    for (int i = 0; i < 4; i++)
    {
        cout<<"task "<<id<<": "<< count++<<endl;
        delay(1);
    }
}

void task2(int id){
    for (int i = 0; i < 6; i++)
    {
        cout<<"task "<<id<<": "<< count++<<endl;
        delay(1);
    }
}

int main(){
    thread t1(task1,1);
    thread t2(task2,2);

    t1.detach();
    t2.detach();

    //t1.join();

    int dem = 0;
    for (int i = 0; i < 3; i++)
    {
        cout<<"task in main "<<": "<< dem++<<endl;
        delay(1);
    }
}

```

**kĩ thuật bất đồng bộ**
Kĩ thuật này nó tương đối giống với detach() tức là thực hiện các luồng mà luồng chính không cần phải chờ nó hoàn thành hết, mà luồng chính vẫn sẽ tiếp tục chạy. Kĩ thuật này nhằm cho việc không bị nghẽn main(), main() cứ chạy thoải mái đồng thời vẫn sẽ chạy các thread khác. Và đặc biệt kĩ thuật bất đồng bộ này có thể đọc được dữ liệu trả về. Các thành phần chính hỗ trợ lập trình bất đồng bộ: 
- std::async : khởi chạy 1 task vụ bất đồng bộ và trả về đối tượng
- std::future: được sử dụng để lưu trữ dữ liệu được trả về được hoàn thành trong tương lai của 1 vụ (lưu ý chỉ 1 tác vụ và đúng tác vụ mà nó lấy của cái async kia kìa)
- std::shared_future: cũm lưu kết quả trả về, nhưng mà nó sẽ chia sẻ cho nhiều luồng khác, còn future chỉ share cho 1 luồng.

Thì nói đơn giản về future và shared_future sẽ như này, future chỉ lấy get() được 1 lần thì :))) chỉ có 1 thread là gọi được 1 lần của nó, còn share_future thì gọi được nhiều lần thì có thể nhiều thread sẽ dùng được thoi.


- async sẽ có 2 chế độ chạy thứ nhất launch::async nó sẽ ngay lập tức chạy cái hàm mà mình truyền vào. Còn chế độ chạy thứ 2 là lauch::deffered nó sẽ không chạy lập tức mà nó chỉ chạy khi có yêu cầu đó là future.get() thì nó mới bắt đầu chạy hàm.

Dưới đây là code thì mình sẽ lưu ý ở đây là khi đến sensor_future.get() thì main() sẽ dừng lại để chờ, chờ cho cái kết quả sensor_future.get() được trả về. Và quên không nói là khi .join() là ta cũm sẽ khiến chương trình main() dừng lại ở đó. Nếu để join() ở đầu thì đằng sau sẽ không chạy, còn nếu ở cuối thì main() sẽ vẫn chạy // với thread khác.

Ngaoif ra mình sẽ lưu ý các syntax như future<int> hay async(launch::async, ...)

```C++
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <future>

using namespace std;

int return_data = 100;
atomic<int> count(0);
mutex mtx;



void delay(int time){
    this_thread::sleep_for(chrono::seconds(time));
}

int sensor_data(){
    for (int i = 0; i < 6; i++)
    {
        mtx.lock();
        cout<< "Read sensor "<< i <<endl;
        return_data = return_data - 5;
        cout<< "Data = "<<return_data<<endl;
        mtx.unlock();
        delay(1);
    }
    
    lock_guard<mutex> lock(mtx);
    cout<<"Read data done"<<endl;
    return return_data;
    
}

void task1(int id){
    for (int i = 0; i < 10; i++)
    {
        mtx.lock();
        cout<<"task "<<id<<": "<< count++<<endl;
        mtx.unlock();
        delay(1);
    }
}

void task2(int id){
    for (int i = 0; i < 10; i++)
    {
        mtx.lock();
        cout<<"task "<<id<<": "<< count++<<endl;
        mtx.unlock();
        delay(1);
    }
}

int main(){

    future<int> sensor_future = async(launch::async, sensor_data);

    thread t1(task1,1);
    thread t2(task2,2);

    int dem = 0;
    for (int i = 0; i < 5; i++)
    {
        mtx.lock();
        cout<<"task in main "<<": "<< dem++<<endl;
        mtx.unlock();
        delay(1);
    }

    int result = sensor_future.get();
    cout << "Data da hoan thanh: "<<result<<endl;

    t1.join();
    t2.join();
}

```

**Những vấn đề trong thread**
- Race condition: là vấn đề khi nhiều thread dùng chung 1 tài nguyên như ghi vào terminal và nó không có sự đồng bộ nên nó sẽ sinh ra việc các thread có thể conflict dẫn đến dữ liệu đầu ra bị sai lệch. Cách giải quyết là sử dụng mutex nhằm phân bổ tài nguyên cho các thread.
- Data race: thì nó sẽ là 1 phần của race condition, tức là thay vì nhiều thread dùng chung 1 tài nguyên, thì data race sẽ là nhiều thread dùng chung 1 biến, đương nhiên nếu mình không đồng bộ thì cái biến đấy có thể bị miss data hoặc sai... -> phải đồng bộ và phương pháp đồng bộ là atomic.
- Dead lock: Về dead lock thì nó sẽ kiểu nếu mình lạm dụng mutex quá mức nó sẽ sinh ra vấn đề này thì ta sẽ làm 1 ví dụ như sau:
Như ta có thể thấy là có 2 mutex và mtx1 và mtx2, và khi ta chạy chương trình thì task1 và task2 đều bị dừng sau khi khóa mutex đầu tiên. Và nó không thể thực hiện tiếp được vì lệnh lock_guard tiếp theo nó cần mutex còn lại, thì bị khóa bởi thread còn lại. -> đây là deadlock. Cách giải quyết là sắp xếp lại hoặc sử dụng try_lock()
```C++
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx1;
mutex mtx2;

void task1(){
    lock_guard<mutex> lock1(mtx1);
    cout<< "Lock mtx1 task1"<<endl;
    this_thread::sleep_for(chrono::seconds(1));

    lock_guard<mutex> lock2(mtx2);
    cout<<"Lock mtx2 task1"<<endl;
    
}

void task2(){
    lock_guard<mutex> lock1(mtx2);
    cout<< "Lock mtx2 task2"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
    
    lock_guard<mutex> lock2(mtx1);
    cout<<"Lock mtx1 task2"<<endl;
    
}

int main(){
    thread t1(task1);
    thread t2(task2);

    t1.join();
    t2.join();
}
```


**Thread Pool**
Là kỹ thuật quản lý tài nguyên luồng, trong đó là 1 nhóm luồng đã được khởi tạo sẵn và nó sẽ chờ để thực thi các task. Khác với thread bình thường là khi dùng xong nó sẽ bị hủy luôn, còn đây các thread sẽ được giữ cho đến hết chương trình. Dễ hình dung sẽ là ta sẽ khởi tạo cố định 5 thread chẳng hạn và chỉ sử dụng 5 cái thread ấy cho tất cả các task trong chương trình.

```C++
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

class ThreadPool{
    private:
    vector<thread>  workers;

    queue<function<void()>> tasks;

    mutex mtxThread;

    bool stop;

    condition_variable cv;
    
    public:
    ThreadPool(const int &number) : stop(false){
        for (int i = 0; i < number; i++)
        {
            workers.emplace_back([this](){
                while (1)
                {
                function<void()> task;
                {
                    unique_lock<mutex> lock(mtxThread);
                    this->cv.wait(lock, [this](){return this->stop || this->tasks.empty()!=0;});
                    if(this->stop == true && this->tasks.empty()) return;
                    task = move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
            });
        }
        
    }

    void enqueue(function<void()> f){
        {
            lock_guard<mutex> lock(mtxThread);
            this->tasks.push(move(f));
            this->stop = true;
        }

        cv.notify_one();
    }

    ~ThreadPool(){
        for(auto &worker : this->workers){
            worker.join();
        }
    }
};
mutex mtx;

int main(){
    ThreadPool tpool(5);
    for (int i = 0; i < 10; i++)
    {
        tpool.enqueue([i](){
            {
                lock_guard<mutex> lock(mtx);
                cout<<"Task "<<i<<"this is thread "<<this_thread::get_id()<<endl;
            }
            this_thread::sleep_for(chrono::seconds(1));

            {
                lock_guard<mutex> lock(mtx);
                cout<<"Task "<<i<<"thread ending is "<<this_thread::get_id()<<endl;
            }
        
        });
    }
}
```

</details>