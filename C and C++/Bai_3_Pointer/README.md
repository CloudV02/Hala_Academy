1. Khái niệm về Pointer 
- Pointer là 1 biến chứa địa chỉ của 1 biến khác. Pointer có kích thước dựa vào kiến trúc máy tính và trình biên dịch.
VD: Với kiến trúc trên máy tính laptop 64bit thì size của sizeof(pointer) = 8 byte
- Kiểu dữ liệu khai báo biến pointer sẽ là kiểu dữ liệu của giá trị tại địa chỉ đang được trỏ tới.

2. Các loại Pointer
- Void Pointer: Dùng để trỏ tới 1 địa chỉ mà không cần biết kiểu dữ liệu của giá trị tại địa chỉ được trỏ tới. Nhưng muốn sử dụng giá trị trong địa chỉ đó thì phải ép kiểu về đúng kiểu dữ liệu của giá trị. 
- Function Pointer : Dùng để trỏ tới 1 hàm trong chương trình. Nó thường được sử dụng khi ta muốn 1 hoặc nhiều hàm làm tham chiếu của 1 hàm khác -> Dùng để viết thư viện. 
- Pointer to Constant :  Định nghĩa 1 con trỏ không thể thay đổi giá trị của địa chỉ được trỏ đến. Muốn thay đổi được giá trị phải thay đổi đúng cái biến chứa giá trị đó.
- Constant Pointer: Định nghĩa 1 con trỏ địa chỉ nó trỏ tới sẽ không thể thay đổi. Tức là biến con trỏ này sẽ không thể trỏ tới 1 địa chỉ khác mà chỉ sử dụng dc 1 địa chỉ ban đầu.