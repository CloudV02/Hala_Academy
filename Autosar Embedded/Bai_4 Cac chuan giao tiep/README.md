1. Truyền nhận dữ liệu
- Truyền nhận dữ liệu là các tín hiệu điện áp biểu diễn cho các bit. Với điện áp 0V tương ứng với bit 0,điện áp 3.3V hoặc 5V biểu diễn cho bit 1. Và vi điểu khiển sẽ chuyền 1 chuỗi các bit 0,1 qua các chân của vi điều khiển, để truyền nhận dữ liệu
VD: Có 2 MCU, 1 MCU A truyền dữ liệu và 1 MCU B nhận dữ liệu cungf qua pin 0 thì con MCU A muốn truyền dữ liệu qua con B thì con A sẽ ghi cái điện áp tương ứng với bit muốn truyền lên chân pin 0 và con B sẽ đọc pin 0 để nhận dữ liệu

- Chuyền 1 chuỗi thì mã hóa thành nhị phân để truyền. VD: Chữ 'h' = 68
- Nếu truyền 1 dây thì truyền từng bit 1 trong chuỗi bit -> ưu điểm đỡ tốn tài nguyên. Còn truyền nhiều dây nhanh nhưng tốn dây

- Làm sao để MCU B phân biệt được các bit liền kề ? -> Để phân biệt được thì có phải sự thôngs nhất giữa các vi điều khiển như sử dụng clock, mỗi 1 chu kỳ clock thì sẽ check lại tín hiệu điện áp trên chân truyền nhận -> các chuẩn giao tiếp ra đời để phân biệt các bit liền kề nhau

- Vì các chuẩn giao tiếp này sẽ liên lạc với nhau giữa các MCU hoặc với thiết bị khác có cùng chuẩn giao tiếp, -> MCU sẽ truyền nhận dữ liệu dễ dàng hơn.


2. SPI
- SPI (Serial Peripheral Interface) là chuẩn giao tiếp nối tiếp đồng bộ và hoạt động được ở chế độ song công tức là có thể vừa truyền vừa nhận cùng 1 lúc.
+ Nối tiếp đồng bộ tức là các bit sẽ truyền trên 1 dây nối tiếp nhau và đồng bộ cùng 1 xung clock do master chỉ định clock đó

- SPI cho phép 1 Master giao tiếp tới nhiều Slave khác nhau. Các dây cùng tên sẽ nối với nhau còn dây SS là dây để nhận dạng SPI Slave nên sẽ nối dây SS tương ứng vào chân SS của SPI Slave tương ứng (tức là các chân SS sẽ nối riêng biệt nhau). Có bao nhiêu Slave sẽ có bấy nhiêu chân SS

- SPI sử dụng 4 dây giao tiếp gồm:
+ SCK (Serial Clock): chân này chịu trách nhiệm đồng bộ. Chân này sẽ điều khiển bởi Master để ghi tín hiệu ra. Master sẽ tạo xung tín hiêuj để cấp cho các Slave khác
+ MOSI (Master Output Slave Input): Chân này chịu trách nhiệm truyền dữ liệu từ Master tới Slave.
+ MISO (Master Input Slave Output): Chân nayf chịu trách nhiệm truyền dữ liệu từ Slave về Master
+ CS / SS (Chip Slave / Slave Select): Chân này điều khiển bơi thằng Master. Chân này giúp Master chọn được vi điều khiển để giao tiếp vì 1 Master có thể có nhiều Slave nhưng trong 1 thời điểm nó chỉ chọn được 1 Slave để giao tiếp và nó chọn bằng chân SS -> Chân SS sẽ nối 1 Slave riêng. Muốn giao tiếp với chân Slave nào thì kéo chân SS tương ứng với Slave đó về mức 0.

- Quá trình truyền nhận SPI: 
+ Mỗi khi truyền dữ liệu đi thì chân SCK sẽ tạo 1 xung clock. Để tạo ra xung clock thì đầu tiên chân SCK sẽ ở mức 0. Khi tryền dữ liệu đi sẽ ghi cái chân MOSI thành cái bit nó muốn truyền đi (tương tự với chân MISO nếu ở quá trình nhận) đồng thời nó kéo chân SCK lên mức 1 và delay 1 đoạn nhỏ rồi kéo lại chân SCK về mức 0. Bên nhận sẽ dựa vào cái xung clock để nhận biết 1 bit hay là 2 bit vì chỉ cần 1 xung clock là nó nhận biết được đó là 1 bit
-> cần phải 1 cái buffer để lưu data -> cần phải đọc các chân SCK và MOSI ở bên nhận (or MISO đối với Master là bên nhận)
+ Nếu trong truyền nhận nhiều Slave thì phải kéo cái chân CS muốn truyền nhận về mức 0 trước

- Các chế độ hoạt động: Có 4 chế độ hoạt động dựa vào 2 bit CPOL và CPHA
+ CPOL quyết định điện áp ban đầu của SCK xem nó phải kéo lên hay kéo xuống để thực hiện 1 xung clock
+ CPHA là thời điểm bit được truyền đi trong 1 xung, pha của tín hiệu

3. I2C
- Là chuẩn giao tiếp nối tiếp và đồng bộ sử dụng 2 dây SDA và SCL. Dùng được 1 master giao tiếp được nhiều Slave và truyền bán song công (tức là trong 1 thời điểm chỉ truyền hoặc chỉ nhận)
- Các slave đều được nối chung trên dây SDA và SCL và có 1 điện trở kéo lên 

- I2C data frame
+ Việc truyền nhận cũng giống như SPI í là mỗi bit ở chân SDA được truyền đi sẽ kèm với 1 xung ở SCL
+ Data frame sẽ bắt đầu bằng Start condition tín hiệu bắt đầu SDA sẽ được kéo thấp xuống trước SCL. Tương tự với trạng thaí Stop condition là trạng thái kết thúc của 1 frame truyền thì SDA sẽ đướcj kéo lên trước chân SCL.
+ Trong frame truyền sau khi bắt đầu sẽ là 7-10 address bit ,mỗi Slave sẽ có 1 cái địa chỉ riêng 1 cái id riêng. Nên giờ Master muốn truyền nhận cho Slave nào phải gửi 1 cái địa chỉ để xác định Slave muốn truyền nhận. Khi truyền xong địa chỉ cần xác định sẽ có 1 bit R/W để cho lệnh đọc hoặc ghi vào Slave của Master. Sau bit R/W sẽ là bit ACK. Vì Master và Slave chỉ truyền trên 1 pin vậy nên khi master truyền thì tất cả các Slave đều nhận được dữ liệu, nên sau khi nhận 7 bits address thì các Slave phải tự hiệu đươcj đấy có phải địa chỉ của nó hay không và gửi thông báo về cho Master biết -> bit ACK là để nhânj thông báo đó. Khi đó ACK đang ở mức 1 sẽ kéo về mức 0 để thông báo cho Master biết là có 1 Slave đã xác nhận. Tiếp theo Master sẽ đến 8 bits data truyền nhận và cũng có 1 bit ACK để thông báo cho Master về việc truyền nhận hoàn tất.

4. UART (Universal Asynchrous Receiver Tranmister)
- Là 1 giao thức truyền thông nối tiếp, không đồng bộ(vì không có dây Clock) 
- Nó gồm 2 chân RX và TX, hoạt động song công (vừa truyền vừa nhận) và nó là giao thưcs chỉ 2 thiết bị giao tiếp với nhau
- Vì không có chân Clock để báo hiệu khi nào truyền nên nó phải sử dụng cách căn thời gian. Nó sẽ định 1 giá trị chung để căn ngay khi con này truyền xong thì con còn lại sẽ nhận thì sau khoảng thời gian đó sẽ là 1 bit
- 2 chân Tx, Rx sẽ ở mức 1 kéo xuống để bắt đầu
- Cách truyền:
+ Thì con MCU truyền sẽ gửi 1 tín hiệu start và sử dụng 1 cái timer để thiết lập thời gian truyền dữ liệu. Thì bên nhận lúc ấy cũng nhận được cái tín hiệu start của bên truyền và cũng thiết lập 1 thời gian nhận tương ứng với thời gian truyền. Để khi MCU này truyền thì MCU kia cũng sẽ nhận.
+ Nhưng mỗi tốc độ chạy của MCU khác nhau như có con chạy 72Mhz, con 32Mhz -> Timer sẽ khác nhau -> Sinh ra Baudrate để thôngs nhất tốc độ truyền hay số bits truyền được trên 1s -> 2 con MCU sẽ thống nhâts Baudrate

- Data frame:
+ Có 1 bit start -> 5 to 9 bits data -> 0 to 1 parity -> 1-2 bit stop
Các bit này sẽ truyền dựa vào baudrate đã quy định hay chính là timer đã quy định giữa 2 con MCU
+ Bit parity là bit quy luật chẵn lẽ để check các bit data trước đó. 2 con MCU sẽ thống nhất quy luật chẵn lẻ của bit parity. Giá trị bit partity cuar bên truyền sẽ phụ thuộc vào số bit 1 trong bit data. Còn bit parity của bên nhận sẽ kiểm tra bit parity của bên truyền của khớp vơis mình không.