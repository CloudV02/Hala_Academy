<details>
<summary><h1> GPIO REGISTER </h1> </summary>

## Một số các define trong thư viện của GPIO

Giờ ta sẽ đi phân tích các define đó:
- Các GPIO: như GPIOA, GPIOB, GPIOC, ... sẽ được định nghĩa như nào. Thì để các GPIOA có thể trỏ tới như GPIOA->CRL thì GPIOA này cần phải là struct.

- Các thuộc tính của GPIO sẽ được lưu vào trong 1 GPIO_InitTypeDef gồm có chọn Pin, có mode của Pin đấy là in hay out kiểu vậy và Speed của Pin.
+ Ta phải define cả Pin cho nó trừu tượng như GPIO_Pin_10  0x0400
+ Mode và Speed sẽ có 1 cái enum để trừ tượng tên gọi

- Đó thì ta thấy cái luồng để tạo define như sau đầu tiên ta thấy ta cần 1 cái trừu tượng như GPIOA trỏ tới CRL chẳng hạn thì như phần code ví dụ 1 bên dưới ta đã trình bày rõ ràng. Nhưng nhìn tổng quan là ta sẽ muốn 1 cái hàm hay cái struct dùng để mình điền cài Pin gì vào thì thiết lập cái pin đó thì dùng GPIO_InitTypedef struct. Kiểu như struct này sẽ chứa việc mình thiết lập Pin nào dùng trong hệ thống, còn cách dùng ra sao thì dựa vào hàm khác như GPIO_SetPin chẳng hạn và ở trong đó mình trỏ GPIOA->ODR chẳng hạn.


## GPIO Mode:

- GPIO_Mode_AIN: Analog Input, chân được cấu hình làm đầu vào là analog -> chân ấy sẽ sử dụng cho ADC.

- GPIO_Mode_IN_FLOATING: Floating Input, trạng thái thả nổi tức là chân được cấu hình là đầu vào, nhưng nó không được thiết lập cố định ở mức cao hay mức thấp, mà nó ở giữa giữa.

- GPIO_Mode_IPD: Input with Pull-down, chân GPIO được cấu hình là chân vào và nó sẽ có 1 điện trở nội bộ kéo xuống GND được kích hoạt. Khi không có tín hiệu nào được áp dụng lên chân này thì nó sẽ ở mức thấp / mức 0.

- GPIO_Mode_IPU: Input with Pull-up, chân GPIO được cấu hình là đầu vào và được nối với điện trở kéo lên. Khi không có tín hiệu nào được áp dụng lên chân này, nó sẽ được kéo lên mức cao / mức 1.

Note muốn biết điện áp đo dòng điện kiểu gì khi có trở kháng cao thì xem nguyên lý hoạt động của vôn kế. Và thêm nữa, dòng điện phải chạy nếu có mạch kín, còn điện áp thì nó vẫn sẽ ở nguyên đó, giống như điện ở trong nhà vẫn luôn có kể cả mình không dùng.

- GPIO_Mode_Out_OD: Open-drain Output, ở chế độ này chân được phép kéo xuống mức thấp bằng phần mềm, còn muốn kéo lên ở mức high thì nối điện trở kéo lên ngoài. Tức là ở trong mạch sẽ không có điện trở kéo lên chỉ có kéo xuống -> phải lắp ngoài. 

- GPIO_Mode_Out_PP: Push-pull Output, thì chân này là đầu ra và mình có thể thiết lập ở cả mức cao và thấp mà không cần phần cứng nào.

- GPIO_Mode_AF_OD: Alternate Function Open-drain, chân GPIO sẽ được cấu hình để hoạt động trong 1 số chức năng thay thế như UART, I2C,... và sử dụng chế độ open-drain -> chỉ sử dụng như output.

- GPIO_Mode_AF_PP: Alternate Function Push-Pull, chân GPIO sẽ được cấu hình để sử dụng các chức năng thay thế như UART,I2C và sử dụng chế độ push-pull.

Ví dụ 1 như sau: Như ta thấy cái địa chỉ của GPIOA_BASE đã được ép kiểu thành 1 con trỏ struct GPIO_Typedf. Vậy nên mỗi lần gọi GPIOA nghĩa là gọi đến cái địa chỉ GPIOA_BASE và nó là struct nên có thể trỏ vào từng member. Ngoài ra __IO là 1 định nghĩa trong thư viên CMSIS, nó có nghĩa là volitile.
```C
#define GPIO_Pin_0          ((uint16_t)0x0001)
#define GPIO_Pin_1          ((uint16_t)0x0002)
#define GPIO_Pin_2          ((uint16_t)0x0004)
#define GPIO_Pin_3          ((uint16_t)0x0008)
#define GPIO_Pin_4          ((uint16_t)0x0010)
#define GPIO_Pin_5          ((uint16_t)0x0020)
#define GPIO_Pin_6          ((uint16_t)0x0040)
#define GPIO_Pin_7          ((uint16_t)0x0080)
#define GPIO_Pin_8          ((uint16_t)0x0100)
#define GPIO_Pin_9          ((uint16_t)0x0200)
#define GPIO_Pin_10         ((uint16_t)0x0400)
#define GPIO_Pin_11         ((uint16_t)0x0800)
#define GPIO_Pin_12         ((uint16_t)0x1000)
#define GPIO_Pin_13         ((uint16_t)0x2000)
#define GPIO_Pin_14         ((uint16_t)0x4000)
#define GPIO_Pin_15         ((uint16_t)0x8000)
typedef struct{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
}GPIO_Typedef; //Typedef thanh ghi
#define GPIOA_BASE (0x40000545UL)
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

typedef enum{
    GPIO_Speed_10Mhz = 0x1;
    GPIO_Speed_2Mhz  = 0x2;
    GPIO_Speed_50Mhz = 0x3;
}GPIOSpeed_Typedef;

typedef enum{
    GPIO_Mode_AIN = 0x00;
    GPIO_Mode_IN_FLOATING = 0x04;
    GPIO_Mode_IPD = 0x28; // đáng lẽ là 0x08 là đủ input rồi nhưng mà thêm 2 và 4 như bên dưới vì trong GPIO_Init mà CMSIS cung cấp cho ta ấy thì nó sẽ dựa vào bit 0x28 và 0x48 để set ODR tương ứng trog cái hàm GPIO_Init.
    GPIO_Mode_IPU = 0x48;
    GPIO_Mode_Out_OD = 0x14;
    GPIO_Mode_PP = 0x10;
    GPIO_Mode_AF_OD = 0x1C;
    GPIO_Mode_AF_PP = 0x18;
}GPIOMode_Typedef;

typedef struct{
    uint16_t GPIO_Pin;
    GPIOSpeed_Typedef   GPIO_Speed;
    GPIOMode_Typedef    GPIO_Mode;
}GPIO_InitTypedef; //Typedef hoat dong

int main(){

}

```
</details>

<details>
<summary> <h1> NGẮT VÀ TIMER </h1> </summary>

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
nói đơn giản thì timer là 1 bộ đếm (đếm lên hoặc xuống) bên trong Timer, ngoài ra nó có thể ở chế độ counter, tức là mình sẽ tác động ngoại vi vào như mỗi lần nhấn nút thì CNT sẽ tăng lên 1 chẳng hạn. Thì các TIMx_CH chính là các chân đầu vào đó. Và 1 Timer chỉ sử dụng được 1 chế độ, 1 là đếm nội là đếm kiểu liên tục í làm delay các thứ, 2 là như chế độ counter đếm dựa vào ngoại vi. Việc lựa chọn clock nội với xung ngoài sẽ dựa vào thanh ghi SMCR thì như ta thấy trong tài liệu thì value reset là 0x0000 tức là nó sẽ auto sử dụng clock nội.

**Giờ ta sẽ nói về 1 số vấn đề cần tìm hiểu trong TIMER**
- Prescaler: là bộ chia tần số. Thì nó để làm gì? Thì nói đơn giản như cái tên của nó là dùng để chia tần số. Ví dụ như trong Timer thì tần số nó được cấp mặc định dựa vào file system.c của hệ thống, thì nó được cấp 72Mhz. Giờ mà ta để nguyên 72Mhz và cho nó đếm thì nó sẽ đếm tràn thanh ghi rất nhanh. Nói thêm về đếm tràn tức là cái thanh ghi giữ giá trị đếm sẽ bị tối đa. Ví dụ như ở TIM2 là 16bit và sẽ có dải đếm 0->65535 và nó đếm quá 65535 là bị tràn và phải quay về 0. Vậy tức là nếu để nguyên 72Mhz tức là thời gian nó đếm 1 tick là đếm từ 0 lên 1 hay từ 1 lên 2 trong thanh ghi đếm của TIM kia sẽ là 1/72.000.000hz, thì việc đếm quá nhanh khiến ta không thu được số liệu gì vì ví dụ nó đếm xong reset, thì mình không biết nó đếm được bao nhiêu. Giờ ta sẽ đưa ra ví dụ giống như đồng hồ đếm từ 0->59 xong lại reset, nó sẽ giống với chạy từ 0->65535, còn 0->1 đối với đồng hồ là nó sẽ được thiết lập là 1s, còn đối với vi điều khiển như mình nói bên trên sẽ là 1/72.000.000. Thì nếu ta thiết lập tần số quá nhanh thì cái dải từ 0->65535 nó cũm bị tràn nhanh, kiểu các con số trong đấy nó lấy ra không có nhiều ý nghĩa. Vậy nên ta cần phải kéo giảm tần số lại để giảm thời gian, nó tràn. Thì ví dụ như cái đồng hồ kia từ 0->1 là ta biết đó là 1s. Thì giờ trong vi điều khiển ta sẽ cố gắng thiết lặp sao cho 0->1 sẽ là 1ms chẳng hạn. Và từ đó bộ chia tần ra đời để làm thay đổi cái tần số thay vì từ 0->1 chạy trong 0.00000072 thì giờ thành 0.0001 thoi, chậm hơn sẽ ứng dụng thực tế nhiều hơn.

- Time Clock: hay thanh ghi CNT, nó sẽ dựa vào mỗi xung hay mỗi tick của clock để đếm lên 1, nó giống với cái 0->65535 t ví dụ ở trên đóa.

- Auto Reload Value: hay ARR là giá trị tối đa mà timer sẽ đếm trước đó, nếu vượt quá giá trị mình set cho ARR nó sẽ quay về 0. Và CNT là thanh ghi sẽ quay về 0.

Tổng kết: Thì Prescaler(bộ chia tần) sẽ quyết định tốc độ nhảy từ 0->1 của giá trị trong thanh ghi CNT, còn ARR sẽ quyết định giá trị tràn của CNT, ví dụ như ARR = 1000, thì CNT sẽ đếm đến 1000 rồi quay về 0, với tốc độ = 1000*(thời gian mà nó nhảy lên 1 tick).

**Nói về Systick timer**
Thì tại sao lại nói về systick timer thì cơ bản timer của systick hơi khác 1 chút, không giống như bộ TIMER thì timer trong Systick không có bộ chia tần.
Thì trong Systick cơ bản sẽ 3 thanh ghi 
- CTRL: thì đây là thanh ghi điều khiển để enable, interrput hay clock source cho Systick, chứ nó không trực tiếp điều khiển timer. 
- LOAD: đây là thanh ghi quan trong để set timer, thì mình sẽ nạp giá trị ở đây. Và ở đây quy ước 16000000 là 1s mà ta có 1s = 1000ms -> 16000 là 1ms. Đó còn nếu /10^6 là us, ncl chỗ này t không biết phải giải thích ki hơn như nào :)) Như kiểu ví dụ vui vui giờ t thích cho là 5k là đổi lấy 1s cuộc đời, thì suy ra 5xu là đổi được 1ms cuộc đời :)) (1s = 1000ms tương đương 5k = 5000xu -> 1000ms = 5000xu -> 1ms = 5xu :)) đơn vị đổi time thoi) 
- VAL: là lấy giá trị đếm hiện tại, tức là khi ta điền LOAD là nó sẽ đếm từ giá trị LOAD đó về 0. Còn VAL nó chỉ đơn giản là để lấy 1 giá trị hiện tại nó đang đếm thoi.

</details>
<details>
<summary> <h1>CÁC CHUẨN GIAO TIẾP</h1></summary>

<details>
<summary> <h2> 1. Truyền nhận dữ liệu </h2> </summary>
- Truyền nhận dữ liệu là các tín hiệu điện áp biểu diễn cho các bit. Với điện áp 0V tương ứng với bit 0,điện áp 3.3V hoặc 5V biểu diễn cho bit 1. Và vi điểu khiển sẽ chuyền 1 chuỗi các bit 0,1 qua các chân của vi điều khiển, để truyền nhận dữ liệu
VD: Có 2 MCU, 1 MCU A truyền dữ liệu và 1 MCU B nhận dữ liệu cungf qua pin 0 thì con MCU A muốn truyền dữ liệu qua con B thì con A sẽ ghi cái điện áp tương ứng với bit muốn truyền lên chân pin 0 và con B sẽ đọc pin 0 để nhận dữ liệu

- Chuyền 1 chuỗi thì mã hóa thành nhị phân để truyền. VD: Chữ 'h' = 68
- Nếu truyền 1 dây thì truyền từng bit 1 trong chuỗi bit -> ưu điểm đỡ tốn tài nguyên. Còn truyền nhiều dây nhanh nhưng tốn dây

- Làm sao để MCU B phân biệt được các bit liền kề ? -> Để phân biệt được thì có phải sự thôngs nhất giữa các vi điều khiển như sử dụng clock, mỗi 1 chu kỳ clock thì sẽ check lại tín hiệu điện áp trên chân truyền nhận -> các chuẩn giao tiếp ra đời để phân biệt các bit liền kề nhau

- Vì các chuẩn giao tiếp này sẽ liên lạc với nhau giữa các MCU hoặc với thiết bị khác có cùng chuẩn giao tiếp, -> MCU sẽ truyền nhận dữ liệu dễ dàng hơn.

</details>

<details>
<summary> <h2> 2. SPI </h2> </summary>
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
</details>

<details>
<summary><h2> 3. I2C </h2></summary>
- Là chuẩn giao tiếp nối tiếp và đồng bộ sử dụng 2 dây SDA và SCL. Dùng được 1 master giao tiếp được nhiều Slave và truyền bán song công (tức là trong 1 thời điểm chỉ truyền hoặc chỉ nhận)
- Các slave đều được nối chung trên dây SDA và SCL và có 1 điện trở kéo lên 

- I2C data frame
+ Việc truyền nhận cũng giống như SPI í là mỗi bit ở chân SDA được truyền đi sẽ kèm với 1 xung ở SCL
+ Data frame sẽ bắt đầu bằng Start condition tín hiệu bắt đầu SDA sẽ được kéo thấp xuống trước SCL. Tương tự với trạng thaí Stop condition là trạng thái kết thúc của 1 frame truyền thì SDA sẽ đướcj kéo lên trước chân SCL.
+ Trong frame truyền sau khi bắt đầu sẽ là 7-10 address bit ,mỗi Slave sẽ có 1 cái địa chỉ riêng 1 cái id riêng. Nên giờ Master muốn truyền nhận cho Slave nào phải gửi 1 cái địa chỉ để xác định Slave muốn truyền nhận. Khi truyền xong địa chỉ cần xác định sẽ có 1 bit R/W để cho lệnh đọc hoặc ghi vào Slave của Master. Sau bit R/W sẽ là bit ACK. Vì Master và Slave chỉ truyền trên 1 pin vậy nên khi master truyền thì tất cả các Slave đều nhận được dữ liệu, nên sau khi nhận 7 bits address thì các Slave phải tự hiệu đươcj đấy có phải địa chỉ của nó hay không và gửi thông báo về cho Master biết -> bit ACK là để nhânj thông báo đó. Khi đó ACK đang ở mức 1 sẽ kéo về mức 0 để thông báo cho Master biết là có 1 Slave đã xác nhận. Tiếp theo Master sẽ đến 8 bits data truyền nhận và cũng có 1 bit ACK để thông báo cho Master về việc truyền nhận hoàn tất.
</details>

<details>
<summary> <h2>4. UART (Universal Asynchrous Receiver Tranmister)</h2> </summary>
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

</details>
</details>

<details><summary><h1> MPU </summary></h1>

# MPU (Memory Protection Unit)

Tại phần này mình sẽ lâpj trình về thanh ghi MPU nên mình sẽ tập trung nói chủ yếu về thanh ghi, cách hoạt động của các thanh ghi

## 1.Lý thuyết
Thì MPU như cái tên là nó sẽ bảo vệ vùng nhớ hoặc sở hữu hay làm bất cứ cái gì nó thích đối với các vùng nhớ nó sở hữu. Thì theo ta được biết nó sẽ có 8 region ở cortex m4, các region có thể ghi đè lên nhau và độ ưu tiên sẽ vào region cao nhất region 7, tức là càng region càng bé nếu đè lên thì những cái vùng ghi chung đó sẽ lấy thuộc tính của region cao hơn VD: region 1 và region 4 mà ghi chung vùng nhớ 0x20100 - 0x20110 chẳng hạn, thì cái vùng nhớ đó sẽ sở hữu các đặc điểm của region 4

Cách để tìm lỗi thì khi xảy ra lỗi của MPU nó sẽ nhảy vào MemManage Exception hoặc Hardfauld và lỗi sẽ được thông báo vào thanh ghi MMFSR - MemManage Fault Status Register và nơi sẽ lưu địa gây ra lỗi là MMFAR - MemManage Fault Address Register.

## 2. Thanh ghi

**Thanh ghi MPU_CTRL**
- Bit 2: PRIVDEFENA bit này sẽ cho phép truy cập vào default memory map ở chế độ privileged. Thì nói thêm thì dèault memory map là vùng không được cấp hình bởi MPU, tức là các vùng không nằm trong region nào. Và ở chế độ privileged là sao? Nếu để bit này là 0 thì sẽ không được phép truy cập luôn, tức là kể cả privileged hay unprivileged cũm không được. Còn nếu set lên 1 tức là nếu mình đang ở chế độ privileged sẽ được phép truy cập thỏa mái.

- Bit 1: HFNMIENA bit này cho phép MPU hoạt động trong Hardfault và Memfault. Tức là sao ? Giờ ví dụ Hardfault hay Memfault nó nằm trong vùng quản lý của MPU là không được truy cập đến, tức là không đọc được, hoặc là nó không thuộc region nào của MPU và mình cũm không bật bit PRIVDEFENA lên -> cả 2 trường hợp đó xảy ra thì mình sẽ không bao h vào được hàm Hardfault hay Memfault -> khi có 1 lỗi xảy ra, nó sẽ tìm tới 2 cái hàm này để trả về kết quả fault, nhưng mà mình khóa con mịa nó vào bằng MPU rồi :)) thì giờ nó cố truy cập tới hàm đó, thì nó lại lỗi và nhảy vào hàm lỗi tiếp -> bị lồng lỗi. Vâyj nên bật bit này lên nó sẽ bỏ quả việc kiểm soát 2 hàm Fault này (Hardfault, Memfault).

- Bit 0: Enable bit này chỉ đơn giản là bật MPU.

**Thanh ghi MPU_RNR**: MPU Region Number Register
- REGION: bit 0-7 là nơi chọn region, theo cortex m3,m4 có 8 hoặc 16 vùng, và mình dùng vùng nào thì ghi giá trị tương ứng, và 1 thời điểm chỉ có 1 vùng. Ví dụ chọn region 3 thì ghi số 3 vào -> REGION ở cortex sẽ chỉ được ghi từ 0->7, còn ghi từ 8->255 nó sẽ bị lỗi.

**Thanh ghi MPU_RBAR**: Region Base Address Register

- REGION - bit [3:0]: Chọn vùng nhớ thiết lập giống với thanh ghi MPU_RNR. Và nó sẽ có bit VALID cũm nằm trong thanh ghi này để mình chọn sử dụng region của cái thanh ghi này hay của thanh ghi MPU_RNR.

- Bit 4 - VALID: với bit = 0 xác định region bằng thanh ghi MPU_RNR, bit = 1 xác định region ở thanh ghi này luôn.

- Bit[31:N] - ADDR: Chứa Base Address của Region, tức là địa chỉ bắt đầu của region. Thì nói về N trước là nó sẽ dựa vào SIZE ở cái thanh MPU_RASR bên dưới. Tức là nếu ta để SIZE là 32byte tương đương với để giá trị trong thanh ghi SIZE là 5 (thực tế nó sẽ là 4 nhưng theo công thức sẽ là 4+1 và ta sẽ lấy 2^(4+1) = 32Byte, đọc tài liệu core để hiểu). Và N sẽ được xác định bằng 32log2 = 5 -> N = 5. Câu hỏi đặt ra là địa chỉ nó có giá trị 32bit cơ mà sao chỉ từ 5->31 sao đủ 32bit lưu địa chỉ? Thì ví dụ ở đây N = 8 đi thì tức là nó sẽ từ bit 8->31, tương đương với việc SIZE sẽ là 2^8 = 256Byte, và giờ theo quy tắc là cái địa chỉ base nó phải chia hết cho 256Byte. Và hay cái :)) những cái chia hết cho 256Byte lại có 8 bit đầu tức là bit 0->7 sẽ bằng 0 hết :)) vậy nghĩa là thực tế mình chỉ cần quan tâm đến bit 8->31 xem có những bit nào là 1, còn các bit dưới 8 đều bằng 0 hết. Vậy tức là đối với N bằng bao nhiêu thì các bit từ 0->N-1 sẽ là bit 0. Còn nếu mà mình set các thanh ghi REGION hay VALID trước đó, thì nếu mình muốn lấy địa chỉ thì mình mask lại thui, kiểu t biến để mask lại, hoặc tạo 1 cái define gì đóa.

**Thanh ghi MPU_RASR**: MPU Region Attribute and Size Register.
Thanh ghi này là thanh ghi cấu hình thoi thif nó sẽ có:

- BIT 28 - XN: Bit này có nhiệm vụ là set cho vùng nhớ đó không được phép truy cập nếu mình set lên 1.

- Bit [26-24] - AP: các bit thanh ghi này sẽ cho biết quyền truy cập vào region. À thì vào đọc bảng chứ ghi ra không hết, tại cũm dễ hiểu.

- Bit [21:19,17,16] -TEX,C,B: nói chung là các bit này kiểu setup các thuộc tính nên đọc thẳng luôn trong tài liệu

- Bit 18 - S: Shareable bit thì nó cho phép chia sẻ vùng nhớ trong ứng dụn Multicore. Thì ncl ít dùng :))

- Bit [5:1] - SIZE: Kích thước của Region, thì nói chung bit này quan trọng, thì trước tiên ta sẽ nói về công thức tính size thực tế dựa trên trường này: Region = 2^(SIZE+1) , SIZE ở đây là mình chọn trong cái trường SIZE đó thì thấp nhất là chọn 4, cao nhất là 31.

- Bit 1 -ENABLE : để bật region thoi.

- bit 8-15 -SRD: Subregion tức là cái vùng nhớ mình cấp í sẽ chia ra 8 vùng nhớ nhỏ, ví dụ 256bytes tương đương SIZE = 7 (7+1) thì mỗi subregion sẽ là 32Bytes. Giờ mình ghi bit 1 (nhớ là từng bit đó ví dụ bật bit 7 thì region 7) lên ở bit nào tương ứng với region đó bị disable (hay đồng nghĩa vùng nhớ đấy sẽ không thể xâm phạm ). Ví dụ 0x02 (0b0000 0010) sẽ disable Subregion 1 (32-63Bytes), trong khi đó các sub khác vẫn hoạt động bình thường.

</details>