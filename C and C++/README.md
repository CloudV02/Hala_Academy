
# COMPILER AND MACRO

==============

## 1. Compiler 
là quá trình biên dịch từ mã người dùng sang mã máy. Nó sẽ bao gồm các bước Preprocessor, Compilation, Assembler, Linking
- Quá trình Preprocessor: Là quá trình tiền xử lý sẽ gộp các file source với file thư viện tương ứng (file.c và file.h), và xử lý các chỉ thị tiền xử lý như macro và xóa các command line
                        gcc -E main.c -o main.i
- Quá trình Compilation: là quá trình biên dịch chương trình sang ngôn ngữ hay file assembly
                        gcc -S main.i -o main.s
- Quá trình Assembler: Quá trình này sử dụng trình biên dịch Assembly thay vì Compiler, nó gộp các file.s nếu có và chuyển sang file.o file object tương ứng
                        as main.s -o main.o
- Quá trình Linking: Là quá trình ta gộp tất cả các file.o vào để tạo ra file cuối cùng để thực thi
                        gcc -v -o main main.o //(thêm các file.o nối đuôi main.o nếu có)

## 2. Macro
Là các chỉ thị tiền xử lý được xử lí trong quá trình Preprocessor gồm:
- #include -> để thêm nội dung các source file khác vào chương trình thường là các source .h
- #define -> Dùng để thay thế nội dung này thành 1 nội dung khác mà mình định nghĩa trước khi chương trình biên dịch
- #ifdef, #ifndef, #if, #elif, #else -> Với #if nếu điều kiện if đúng tức là đã được define trước đó đúng với điều kiện thì mã nguồn dưới if sẽ được thực thi, tương tự với #elif và #else. Còn với #ifdef và ifndef -> thì nếu define đấy mà chưa được định nghĩa thì khi sẽ thực thi mã nguồn dưới ifndef. Tương tự với ifdef nếu define đó được định nghĩa rồi thì chạy mã lệnh bên dưới. Cả 2 gặp #endif là sẽ kết thúc

- Macro không phải kiểu dữ liệu nào cả mà cũng có không có kích thước cụ thể vì nó xảy ra ở quá trình tiền xử lý và nó thay thế cái đoạn được định nghĩa vào

- Dùng macro giúp chương trình chạy nhanh hơn nhưng size của chương trình sẽ tăng. Còn dùng function nó sẽ xảy ra quá trình Function Call làm chậm chương trình đặc biệt với funcion được gọi nhiều lần

- Dùng ## để nối chuoi trong macro

- Va_args được sử dụng khi không xác định được tham số đầu vào trong macro. Cấu trúc được viết (...) __VA_ARGS__

# BÀI POINTER:

==============

## 1. Khái niệm về Pointer 
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

## 2. Các loại Pointer
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

# BÀI CÁC BIẾN ĐẶC BIỆT

====================

## 1. Extern
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

## 2. Static
- **Static local**:
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

## 3. Register
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

## 4. Volatile
- Từ khóa này để cho trình biên dịch của chương trình luôn chạy dòng code đó vì nhiều trình biên dịch sẽ tối ưu hóa đoạn code là chỉ chạy 1 lần -> dẫn tới sai sót trong hệ thống nhúng, vì hệ thống nhúng có những exception nên khi quay về chương trình chính sẽ tiếp tục -> có thể gây sai sót. Khi dùng volatile giúp trình biên dịch hiểu là phải chạy dòng code đó lại.

# BÀI GOTO SETJUMP

================

## 1. Goto
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

## 2. Setjmp.h
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

# BÀI BIT MASK
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
# BÀI STRUCT UNION