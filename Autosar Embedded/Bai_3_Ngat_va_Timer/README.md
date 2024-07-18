- Ngắt là 1 sự kiện khẩn cấp xảy ra khiến cho chương trình chính đang thực thi phải dừng lại và nhảy sang thực hiện sự kiện khẩn cấp đó. Và sau khi thực hiện xong sự kiện khẩn cấp nó sẽ quay về chương trình chính để tiếp tục.

- Các loại ngắt:
Mỗi ngắt đều có 1 trình phục vụ ngắt, tức là sẽ có 1 địa chỉ của ngắt để khi ngắt xảy ra nó sẽ nhảy vào để thực thi. Và các trình phục vụ ngắt sẽ được lưu cố định vào 1 địa chỉ ở trong MCU.
+ Ngắt: Reset -> System Exception
+ Ngắt ngoài, Timer1, Ngăts truyền thông sẽ có cờ ngắt để thông báo rằng sự kiện ngắt đang xảy ra -> Interupt Exception -> Có thể lập trình dc

- Giải thích PC: Đây là 1 thanh ghi nó sẽ trỏ vào địa chỉ của dòng lệnh tiếp theo. Để MCU hiểu là sau khi thực hiện lệnh hiện tại xong, nó sẽ cần phải thực hiện lệnh gì tiếp theo.
- Chuyện gì xảy ra nếu có ngắt ?
Khi có 1 ngắt xảy ra chương trình chính sẽ nhảy vào chương trình ngắt. Tức là con trỏ PC sẽ thông qua Vector Table tìm tới cái địa chỉ đang lưu dữ liệu của chương trình ngắt tương ứng. Khi có ngắt xảy ra các thông tin ở chương trình chính như giá trị của thanh ghi PC(lệnh tiếp theo của lệnh hiện tại thực thi) sẽ được lưu vào Stack Pointer và 1 số các thanh ghi của Core cũng sẽ được lưu vào Stack Pointer. Và khi chương trình ngắt thực hiện xong con trỏ PC sẽ được nhận 1 giá trị thoát khỏi chương trình ngắt hiện tại và địa chỉ nhảy ra sẽ được PC ghi lại, tiếp theo là sẽ đẩy các dữ liệu đã lưu trong stack pointer vào trong thanh ghi Core, sau đó sẽ tiếp tục chương trình chính

- Ngắt ngoài: Xảy ra khi có sự thay đổi điện áp ở chân GPIO
+ LOW : kích hoạt ngắt liên tục khi chân ở mức thấp tức là điện áp ở 0V mới kích hoạt ngắt
+ HIGH : kích hoạt ngắt liên tục khi chân ở mức cao
+ Rising : Được kích hoạt khi trạng thái chân được chuyển từ thấp lên cao, tức là không phaỉ ở hẳn trạng thái 0v và 3.3v mà nó ở nằm ở giữa khoảng từ 0v lên 3.3v -> ngăts sẽ được kích hoạt.
+ Falling : Được kích hoạt khi trạng thái chân chuyển từ mức cao xuôngs thấp. Ngược lại với Rising nó nằm ở khoảng 3.3v xuông 0v 

- Ngăts Timer: Ngắt Timer xảy ra khi thanh ghi đếm timer tràn. Timer nó sẽ giống như 1 bộ đếm, nó sẽ đếm lên hoặc đếm xuống sau khoảng thời gian nhất định (do mình config). Giá trị tràn được xác định bởi giá trị cụ thể và mình cx config cho thanh ghi đếm của timer.
VD: Sau 200ms nó sẽ tràn thì mỗi 1ms nó sẽ đếm lên 1 lần và đến 200ms nó sẽ tràn và khi tràn sẽ tạo ra 1 cái ngắt timer.
Vì timer là 1 phần bên trong của con vi điều khiển và nó không phụ thuộc vào bên ngoài nên mỗi lần tràn thì reset lại giá trị = 0, còn nếu không reset thanh ghi nó sẽ tiếp tục đếm vì mình thiết lập nó đếm rồi nó sẽ không dừng lại.

- Ngắt truyền thông: Xảy ra khi có sự khiênr truyền hoặc nhận dữ liệu giữa vi điều khiển và thiết bị bên ngoài or 1 con vi điều khiển khác. Ngắt này sử dụng để đảm bảo truyền nhận được chính xác. VD: 2 con vi điều khiển khác nhau khi con này đang truyền dữ liệu thì con kia đang thực hiện câu lệnh khác không phải lệnh nhận dữ liệu -> sẽ không có sự truyền nhận data giữa 2 vi điều khiển -> sẽ sai. Vậy nên cần ngắt để khi con vi điều khiên A truyền dữ liệu cho vi điều khiển B và thông qua trình quản lý ngắt con vdk B sẽ nhận tín hiệu và sẽ nhảy vào ngắt truyền thông và thực hiện hàm nhận dữ liệu trong đó.

- Độ ưu tiên ngắt: Xác định thứ tự thực thi của ngắt khi có nhiều ngắt xảy ra đôngf thời.Ngắt có độ ưu tiên càng cao thì sẽ được thực hiện trước. Số ưu tiên càng thấp thì quyền càng cao, các ưu tiên ngắt có thể lập trình được

- TIMER: 
Timer là mạch đếm logic nó sẽ đếm sau mỗi chu kỳ nhất định có thể đếm lên hoặc xuống do mình config.