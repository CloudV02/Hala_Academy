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