BÀI 1: MACRO

1. Compiler là quá trình biên dịch từ mã người dùng sang mã máy. Nó sẽ bao gồm các bước Preprocessor, Compilation, Assembler, Linking
- Quá trình Preprocessor: Là quá trình tiền xử lý sẽ gộp các file source với file thư viện tương ứng (file.c và file.h), và xử lý các chỉ thị tiền xử lý như macro và xóa các command line
                        gcc -E main.c -o main.i
- Quá trình Compilation: là quá trình biên dịch chương trình sang ngôn ngữ hay file assembly
                        gcc -S main.i -o main.s
- Quá trình Assembler: Quá trình này sử dụng trình biên dịch Assembly thay vì Compiler, nó gộp các file.s nếu có và chuyển sang file.o file object tương ứng
                        as main.s -o main.o
- Quá trình Linking: Là quá trình ta gộp tất cả các file.o vào để tạo ra file cuối cùng để thực thi
                        gcc -v -o main main.o //(thêm các file.o nối đuôi main.o nếu có)

2. Macro
Là các chỉ thị tiền xử lý được xử lí trong quá trình Preprocessor gồm:
- #include -> để thêm nội dung các source file khác vào chương trình thường là các source .h
- #define -> Dùng để thay thế nội dung này thành 1 nội dung khác mà mình định nghĩa trước khi chương trình biên dịch
- #ifdef, #ifndef, #if, #elif, #else -> Với #if nếu điều kiện if đúng tức là đã được define trước đó đúng với điều kiện thì mã nguồn dưới if sẽ được thực thi, tương tự với #elif và #else. Còn với #ifdef và ifndef -> thì nếu define đấy mà chưa được định nghĩa thì khi sẽ thực thi mã nguồn dưới ifndef. Tương tự với ifdef nếu define đó được định nghĩa rồi thì chạy mã lệnh bên dưới. Cả 2 gặp #endif là sẽ kết thúc

- Macro không phải kiểu dữ liệu nào cả mà cũng có không có kích thước cụ thể vì nó xảy ra ở quá trình tiền xử lý và nó thay thế cái đoạn được định nghĩa vào

- Dùng macro giúp chương trình chạy nhanh hơn nhưng size của chương trình sẽ tăng. Còn dùng function nó sẽ xảy ra quá trình Function Call làm chậm chương trình đặc biệt với funcion được gọi nhiều lần

- Dùng ## để nối chuoi trong macro

- Va_args được sử dụng khi không xác định được tham số đầu vào trong macro. Cấu trúc được viết (...) __VA_ARGS__

BÀI POINTER:

1. Khái niệm về Pointer 
- Pointer là 1 biến chứa địa chỉ của 1 biến khác. Pointer có kích thước dựa vào kiến trúc máy tính và trình biên dịch.
VD: Với kiến trúc trên máy tính laptop 64bit thì size của sizeof(pointer) = 8 byte
- Kiểu dữ liệu khai báo biến pointer sẽ là kiểu dữ liệu của giá trị tại địa chỉ đang được trỏ tới.

2. Các loại Pointer
- Void Pointer: Dùng để trỏ tới 1 địa chỉ mà không cần biết kiểu dữ liệu của giá trị tại địa chỉ được trỏ tới. Nhưng muốn sử dụng giá trị trong địa chỉ đó thì phải ép kiểu về đúng kiểu dữ liệu của giá trị. Được sử dụng khi không xác định được kiểu dữ liệu được trỏ tới
VD: Như trong hình dưới đây sẽ là 1 function pointer với 2 tham số đầu vào const void * . Con trỏ void được sử dụng ở đây vì ta đang không biết tham số được truyền vào là gì, vì đối với struct thì nó sẽ có nhiều kiểu dữ liệu bên trong struct.

<img src="image/void pointer 1.png" alt="Logo" style="width: 100%; height: auto;">



- Function Pointer : Dùng để trỏ tới 1 hàm trong chương trình. Nó thường được sử dụng khi ta muốn 1 hoặc nhiều hàm làm tham chiếu của 1 hàm khác -> Dùng để viết thư viện.
Như ví dụ dưới đây ta có thể thấy sử dụng khai báo 1 function pointer chưa trỏ tới địa chỉ nào cả, và nó sẽ tham sôs cho hàm Cal để khi ta muốn trỏ tới hàm nào chỉ cần nhập hàm đó vào, và biến function pointer sẽ trỏ tới địa chỉ của hàm đó và thực thi.

<img src="image/function pointer 1.png" alt= "Logo" style = "width: 100%; height: auto;"> 
<img src="image/function pointer 2.png" alt= "Logo" style = "width: 100%; height: auto;"> 

- Pointer to Constant :  Định nghĩa 1 con trỏ không thể thay đổi giá trị của địa chỉ được trỏ đến. Muốn thay đổi được giá trị phải thay đổi đúng cái biến chứa giá trị đó.
- Constant Pointer: Định nghĩa 1 con trỏ địa chỉ nó trỏ tới sẽ không thể thay đổi. Tức là biến con trỏ này sẽ không thể trỏ tới 1 địa chỉ khác mà chỉ sử dụng dc 1 địa chỉ ban đầu.

BÀI CÁC BIẾN ĐẶC BIỆT
1. Extern
- Dùng extern để lấy 1 biến hoặc 1 hàm được khai báo global của nguồn file khác vào chương trình hiện tại của mình để sử dụng
- Giúp cho chương trình có thể tách thành các phần nhỏ để dễ dàng quản lý

2. Static
- Static local:
+ Sẽ được khai báo trong 1 hàm và sẽ được cấp phát địa chỉ cho biến đó. Thông thường nếu không sử dụng local static cho biến đó nó sẽ được lưu trong vùng stack và khi thoát khỏi hàm sẽ biến mất, còn nếu sử dụng local static nó sẽ tồn tại trong suốt time chạy chương trình và có 1 địa chỉ cụ thể ở vùng bss nhưng biêns local static chỉ được sử dụng trong hàm mà khai báo biến đó. Nếu muốn sử dụng biến đó bên ngoài hàm, phải cần 1 biến con trỏ global trỏ tới địa chỉ của local static
- Static global: 
+ Nó sẽ được coi như là 1 biến global thông thường trong file nguồn hiện tại, điểm khác duy nhất là các file nguồn khác không thể sử dụng extern để lấy biến đó.

3. Register
- Từ khóa này để cho người lập trình muốn cho 1 biến thường xuyên sử dụng được lưu trữ trong thanh ghi để sử dụng thay vì biến đos được lưu vùng nhớ RAM -> việc này làm tăng tốc độ xử lý của biến đó

4. Volatile
- Từ khóa này để cho trình biên dịch của chương trình luôn chạy dòng code đó vì nhiều trình biên dịch sẽ tối ưu hóa đoạn code là chỉ chạy 1 lần -> dẫn tới sai sót trong hệ thống nhúng, vì hệ thống nhúng có những exception nên khi quay về chương trình chính sẽ tiếp tục -> có thể gây sai sót. Khi dùng volatile giúp trình biên dịch hiểu là phải chạy dòng code đó lại.

BÀI GOTO SETJUMP
1. Goto
- Goto cho phép chương trình nhảy từ lệnh goto đến 1 label đã được đặt trước trong cùng 1 hàm.
- Sử dụng goto giúp thực hiện chương trình nhanh gọn, nhưng sử dụng nhiều khiến chương trình khó quản lý. Nên vì vậy chỉ nên dùng goto với những chương trình có nhiều vòng lặp

2. Setjmp.h
- Là thư viện trong ngôn ngữ C. Thư viện này cung cấp 2 hàm setjump và longjmp. Các hàm này để xử lý ngoại lệ trong C
- Nó khác goto ở chỗ có thể thay đổi điều kiện giống if và nó có thể khai báo toàn cục, trong khi Goto chỉ sử dụng trong cùng 1 hàm 
- Thường dùng để báo lỗi trong chương trình