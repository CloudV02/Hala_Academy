Follow của chương trình:

- Khởi tạo các module gồm LCD hiện welcome

- Cơ chế: Khi quẹt thẻ nếu thẻ đó đúng, LCD sẽ hiện cho qua hay không cho qua và time ,servo quay để mở barrier, đồng sau đó sẽ lưu các thông tin time vào ra của xe vào SD Card.

- Cách chạy: 

+ Với RFID sẽ chờ có ngắt và chạy
+ với LCD trong time chờ có người quẹt sẽ hiện time, khi có người quẹt hiện tên user và time quẹt. LCD có 1 button. Mỗi lần ấn sẽ chuyển đổi giữa 2 chế độ, 1 là màn hình chính hiện time kia, 1 chế độ sẽ hiển thị xe đó đang ở trong hay ở ngoài bãi đỗ
+ với servo chờ RFID gọi, ngoài ra servo còn 1 nút ấn để tự mở thủ công.
+ Với SPI thì khi có thẻ quẹt lấy thông tin thẻ vào sdcard (xe đó vào hay ra và lúc đó là mấy h)

Oke bài RFID sẽ sử dụng ngắt, đảm bảo nhận được dữ liệu, sau đó sẽ lưu dữ liệu vào buffer (FIFO), servo sẽ được ưu tiên nâng lên trong 10s (cho servo sleep 10s). Trong time đó wake LCD, hiển thị LCD đi lên ròi lại cho LCD sleep, tiếp đến wake SD Card, SD card sẽ lấy dữ liệu từ buffer, ghi vào. Nếu trong quá trình ghi vào servo đóng thì sẽ ưu tiên servo (context switching) -> use 3 thread (thread 1: servo được wake bởi button và RFID, thread 2: LCD wake bởi RFID và button, thread 3 wake bởi RFID nhưng mà độ ưu tiên thấp nhất (servo và LCD phải thực hiện trước))


GPIO của các module:
- RFID: GPIOA 4-5-6-7 SPI1
- SD Card: GPIOB 12-13-14-15 SPI2
- LCD: GPIOB 6-7 I2C1
- Servo: GPIOA 0 PWM
- UART: GPIOA 9-10



Module stm32f103
Module esp32 (15x15)
Servo (header 3P) 5V
RFID (header 8P) 3.3V
LCD (header 4P) 5V
SD Card (header 6P) 5V

LED Satus for esp32
Dht11 for esp32 (Header 3P) 

2 tụ, 2 diode và 1 cuộn cảm cho LM2576 (Buck Converter)

AMS1117 (5V - 3.3V)


0x200002F8 
-> (stack 0x20000484)
-> (next TCB 0x2000030C)

0x2000030C
-> (stack 0x20000614)
-> (next TCB 0x20000320)

0x20000320
-> (stack 0x200007A4)
-> next TCB 0x200002F8 

RCC->CR = 0x000000083 (HSI ON)
0x00014083
0001 0100 0000 1000 0011
HSION
HSIRDY
HSITRIM = 8
HSEON


RCC_AHBENR = 0x000014
0x00000014
FLITFEN
SRAMEN

RCC_APB2ENR = 0x000000
0x0000101D



RCC_APB1ENR = 0x00000
0x00004003


RCC_CSR = 0x0000

0x1C000000 


Systick use AHB

