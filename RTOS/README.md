# RTOS


<details><summary><h1> 2.Internals of A Real-Time Kernel on ARM Processors </h1></summary>

Using Keli C to practice this course
Using stm32f411VET board
Oke now u want to run a pin, we have to config like below:
- Enable clock access to Port of the PIN
- Set the Pin's mode
- Set output

**Next**
Ok now u want to LED in Port D12 lighting
- First go to the datassheet stm32f411vet, find the bus clock point to PORTD and have to enable this bus via its register. With Port D, enable AHB1 100mHZ
Now we will open reference manaul and find AHB1 and can see that it in RCC register

```C
/* stm32f411vet board
PD12 - green
PD13 - orange
PD14 - red
PD15 - blue
*/
    /*enable clock for PORT D*/
    RCC -> RCC_AHB1ENR |= 1<<3;j
```
- Second we have to select pin for PortD -> search for GPIO register
+ config I/O direction mode in GPIOx_MODER and now if want to set PD12 or PD13 or PD14 or PD15, have to enable bit similar from 24 to 31 and we can see that 01: is output so we have to bit 01 in the MODER similar.
+ next setup high and low mode of pin -> GPIOx_ODR.

- Được rồi đến phần này ta sẽ dùng tiếng Việt khi nói về SystickTimer. System tick là bộ đếm trong core luôn nên nó sẽ ứng dụng nhiều trong RTOS vì nó sẽ phản hôig nhanh với core. Ngoài ra sẽ có 1 số timer khác nhưng nó không nằm trong core như timer 1,2 ..,RTC, watchdog timer. Điểm khác nhau cơ bản của các loại timer:

+ RCC: Reset and Clock Control dùng để quản lý toàn bộ clock system bằng việc chọn nguồn clock và điều chỉnh tốc độ xung, bật tắt các thiết bị ngoại vi như GPIO, UART, TIM, RTC,.. Có thể nói RCC sẽ cung cấp toàn bộ hệ thống clock cho các thiết bị kể cả các General Timer, RTC, Watchdog thì cũm do RCC phân phối nguồn clock. Nhưng mà vẫn cầm các bộ đếm bên trên vì RCC chỉ là người phân phối điều chỉnh chứ không biết đếm.

+ Systick dùng cho hệ thống core và nó sẽ có hàm systick handler riêng để tạo ra ngắt đếm. Nhưng vẫn sẽ phải phân bổ dựa vào RCC, bus clock của Systick là HCLK dựa vào tài liệu rm0383-stm32f411xce thì nó vânx phải lấy từ các nguồn HSI, HSE, PLLCLK rồi nhờ RCC để phân bổ.

+ General Timer: sử dụng linh hoạt có thể đếm hoặc tạo xung pwm. Đặc điểm nổi bật của Timer là có thể tạo ngắt đếm, nên sẽ sử dụng thường xuyên cho các ngoại vi. Bus clock của bộ Timer sẽ là APB1 hoặc APB2 và 2 đươngs bus này sẽ phụ thuộc vào sự phân bổ của RCC mà lấy nguồn từ HSI, HSE hoắc PLLCLK. Và sau khi chọn xong bus hệ thống thì nó sẽ được gọi là SYSCLK 100mhz max.

+ Watchdog Timer: là 1 ngoại vi riêng biệt tức là nó sẽ hoạt động độc lập ngay cả khi CPU bị lỗi, bị treo. Và nó có tác dụng đếm ngược lại để reset hệ thống nếu chương trình bị treo. Watchdog Timer có clock riêng IWDG(Indep WDG) với clock LSI, rất quan trọng trong hệ thống an toàn.

+ RTC: Real Time Clock như cái tên thì nó là bộ đếm thời gian thực tức là khi mình tắt con vi điều khiển thì nó vẫn sẽ hoạt động để đếm -> thường ứng dụng trong đếm đồng hồ giây - phút - giờ. Bus clock của RTC là LSI 32kHz, LSE 32.768kHz

- Vấn đề đặt ra khi ta chạy code muốn 2 chương trình Orange_main và Blue_main cứ 1s đèn nháy kiểu gì. Thì ta không có cách nào khác ngoài việc OS chúng cả tức là cho chúng chạy // với nhau cứ 1s lại nhảy vào 1 chương trình. Và làm sao để làm được thì ví dụ khi ta tạo 1 while(1) trong hàm Orange_main hoặc Blue_main thì nó cũm mãi không thoát ra được thì phải cần 1 thứ gì đó trung gian, thì như ta được biết stack frame trong phần cứng luôn hoạt động, tức là nơi ram sẽ lưu giữ liệu trong quá trình chạy, và ở đó có 1 con trỏ pc sẽ trỏ tới dòng lệnh tiếp theo khi thoát khỏi hàm. Tức là khi 1 quá trình function call hay 1 interrupt xuất hiện các dữ liệu sẽ được lưu tạm vào trong stack và khi kết thúc hàm nó sẽ lấy dữ liệu từ stack ra để tiếp tục. Từ đó ta sẽ lấy dữ liệu từ con trỏ pc cứ 1s thì ta lại đổi con trỏ pc thành hàm blue hay orange như vậy, sẽ tạo thành 1 vòng lặp. Còn stack frame đọc kĩ hơn ở trong coretex document. Xem kĩ hơn trong video ấy ở folder 2.
- Như ta đã nói ở bên trên việc làm sao để chuyển đổi các hàm với nhau và ta đi tới kết luận là sử dụng con trỏ pc và thay đổi nó để nó chạy tới function khác. Nhưng điều đó sẽ dẫn tới việc chương trình chạy không đúng tức là nó sẽ không được chạy về lại nơi nó đã tạo ra ngắt, vì đây là chương trình đơn giản, nhưng nếu vào chương trình lớn nó có thể gây ra lỗi. Vậy nên chúng ta phải tạo ra register riêng cho orange main và blue main thay vì chỉ thay đổi con trỏ pc trong main stack pointer. Và các thanh ghi riêng này sẽ nằm trên RAM - nó là các vùng nhớ ta tự tạo thôi quy trình là ví dụ ta đang làm việc ở orange_main thì trước khi chuyển task ta sẽ lưu từ top stack pointer vào vùng nhớ oragne_main[40]. Và sau đó stack pointer sẽ lấy hết các giá trị từ vung nhớ blue_main[40] và bắt đầu chạy blue_main() dựa vào con trỏ pc mà blue_main[], tương tự khi chuyển về orange_main. 
+ Ở đây có thuật ngữ context chính là dữ liệu của các thanh ghi CPU được lưu vào trong Stack và sau đó sẽ sao chép dữ liệu từ Stack về vùng nhớ đệm như orange_main[40] hay đó là 1 thanh ghi do mình tạo ra trong chương trình. Và context nó lưu toàn bộ dữ liệu cần thiết để 1 task có thể tiếp tục chạy khi bị ngắt giữa chừng, bao gồm các thanh ghi R0-R12, PC, xPSR. Hoặc Stack Pointer, các biến tạm.


- Và khi ta khởi tạo 1 biến con trỏ sp_blue hay sp_orange nó đã được khởi tạo ngẫu nhiên trên RAM và có kích thước là int[40]. Như trong video khi ta vào hàm orange_main đầu tiên thì mình không nhất thiết phải lưu thanh ghi trước khi vào, vì mình không có sử dụng, nên cứ thế nhảy vào thôi. Ngoài ra các cái giá trị R1-R12 gì đó thì nó sẽ được tự cập nhật trong thanh ghi chính khi mình chạy nên cũm không phải lo lắm. Thêm nữa thanh ghi LR nó sẽ chỉ cập nhật khi nhảy vào hàm khác và nó sẽ sử dụng khi mình thoát khỏi hàm, đối với ứng dụng như trong chương trình thì nó ở trong vòng lạp while(1) nên không thể thoát khỏi hàm -> LR không bị bắt buộc cho giá trị thỏa mái. Và khi nó nhảy vào hàm DelayS() chẳng hạn thì LR nó sẽ cập nhật, nói chung là có while(1) thì cái LR mình khởi tạo kia nó không bao h bị gọi đến luoon.


** Code the knowleadge **
```C
#include "stm32f4xx.h"

#define GREEN           (1U<<12)
#define RED             (1U<<14)
#define ORANGE          (1U<<13)
#define BLUE            (1U<<15)

#define GREEN_BIT       (1U<<24)
#define ORANGE_BIT      (1U<<26)
#define RED_BIT         (1U<<28)
#define BLUE_BIT        (1U<<30)

#define GPIOD_CLOCK     (1U<<3)

uint32_t orange_stack[40]; // tao register save tien trinh cho orange_main()
uint32_t blue_stack[40]; // tao register save tien trinh cho blue_main()

/*sp_orange lấy địa chỉ ngay sau phần bộ nhớ dành cho stack frame
tức là thay vì lấy địa chỉ đầu tiên của top stack đóa là R0 (R0 sẽ được đẩy vào frame đầu tiên), thì ta sẽ lấy địa chỉ gần cuối của frame, nó sẽ gần với xPSR (xPSR được đẩy vào cuối frame). Thì cta lấy được địa chỉ cuối và -- đi.
* này là lấy địa chỉ của &orange_stack[40] nhớ :) chứ không phải lấy địa chỉ mảng đou, lấy địa chỉ mảng *sp_orange = &orange_stack sẽ là như này hoặc là *sp_orange = &orange_stack[0]. 
* Tức là việc đi lùi này sẽ tránh việc mình tràn stack, vì nếu từ orange_stack[0] ++ lên thì có thể bị tràn ra 41-42-43 -> over stackframe. Còn việc lùi thế này thì khi chạm vào orange_stack[-1] mà lỗi luôn. -> code kiểu này rất hợp lí
* Với lại stack frame của ARM nó là last in first out và khi vào quá trình stack frame. Stack Pointer sẽ -32byte (cái này do phần cứng của ARM nó được thiết lập là vậy, cơ chế của stack frame là khi vào quá trình stacking nó sẽ -32byte so với stack pointer) vậy sao lại trừ. Nó trừ để nó ưu tiên 8 cái thanh ghi R0 R1 R2 R3 R12 LR PC xPSR này sẽ được ghi vào trước. Thì lúc này sp_orange = &orange_stack[40] và sp_orange sẽ = sp_orange - 32 byte (cái này do cơ chế sẵn không thể thay đổi). Nó sẽ lưu R0 vào sp_orange - 32 byte, sau đó sẽ lưu R1 vào sp_orange - 28 byte. Tức là xPSR sẽ gần với địa chỉ của &orange_stack[40] là sp_orange - 4 byte. Tức là ghi R0 trước ròi ++ dần tiến gần về địa chỉ sp_orange = &orange_stack[40]. Nma nó lưu vào thành 1 mảng í, nghĩa là stack pointer lúc này vẫn nằm ở địa chỉ chưa R0 tức là stack pointer = sp_orange = &orange_stack[40] - 32 byte. Thì đó giờ cơ chế phần cứng stacking đã xong, giờ nếu t muốn lưu thêm cái dì thì ta sẽ -- đi, tính từ R0.
Thì giờ ta muốn push thêm R4-R11, thì sẽ -- từ R0, xong đưa R11 vào trước xong lại -- đi đến R4 (từ R4 - R11 là 32 byte).
-> Nếu ta ghi hết cả thanh ghi cứng của ARM thì cũm chỉ mất 64 byte. Nma ở dưới ta còn để 160 byte 40*4byte, tức là sau khi ghi hết phần cứng thanh ghi của ARm thì còn thừa 160 - 64 = 96 byte. Thì 96 byte này ta sẽ nghiên cứu sau nhưng cơ bản nó có thể lưu function call, local variable, safety margin ...
*/
uint32_t *sp_orange = &orange_stack[40]; // tao stack pointer cho orange_main()
uint32_t *sp_blue = &blue_stack[40]; // tao stack pointer cho blue_main()

volatile uint32_t tick;
volatile uint32_t _tick;


void GPIO_Init(void);
void DelayS(uint32_t seconds);
void blueOn(void);
void blueOff(void);
void OrangeOn(void);
void OrangeOff(void);
int blue_main(void);
int organe_main(void);
int main(){
    GPIO_Init();
    
    /*Stack for orange_main thread*/
    /*cái này địa chỉ sẽ giảm dần*/
    *(--sp_orange) = (1u<<24) /*xPSR 1<<24 là ở trong thanh ghi 32 bit bit thứ 24 là bit chế độ thumb mode*/
    *(--sp_orange) = (uint32_t)&organe_main; /*PC*/
    *(--sp_orange) = 0x0000000DU /*LR - là thanh ghi trả về kiểu mode như thread mode .. có hẳn 1 page trên rm nói về các giá trị nó sử dụng - ở đây mình fake giá trị khi mới đầu khởi tạo*/
    *(--sp_orange) = 0x0000000DU /*R12 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000EU /*R3 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000AU /*R2 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000DU /*R1 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000AU /*R0 - fake value vì khởi tạo lần đầu*/


    /*Stack for blue_main thread*/
    *(--sp_orange) = (1u<<24) /*xPSR 1<<24 là ở trong thanh ghi 32 bit bit thứ 24 là bit chế độ thumb mode*/
    *(--sp_orange) = (uint32_t)&blue_main; /*PC*/
    *(--sp_orange) = 0x0000000DU /*LR - là thanh ghi trả về kiểu mode như thread mode .. có hẳn 1 page trên rm nói về các giá trị nó sử dụng - ở đây mình fake giá trị khi mới đầu khởi tạo*/
    *(--sp_orange) = 0x0000000DU /*R12 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000EU /*R3 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000AU /*R2 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000DU /*R1 - fake value vì khởi tạo lần đầu*/
    *(--sp_orange) = 0x0000000AU /*R0 - fake value vì khởi tạo lần đầu*/
    while(1){
        
    }

void GPIO_Init(void){
    /*Set clock for PORT D*/
    RCC->RCC_AHB1ENR |= GPIOD_CLOCK;
    
    /*Set pin for PORT D*/
    GPIOD->MODER |= GREEN_BIT | ORANGE_BIT | RED_BIT | BLUE_BIT;
    SystemCoreClockUpdate(); // hàm dùng để cập nhật lại biến toàn cục SystemCoreClock vì mình không biết giá trị của bus clock cấp cho core hiện tại là bao nhiêu nên phải update.

    SysTick_Config(SystemCoreClock/100u); /* nếu chia 1000 sẽ tạo ra 1ms ngta để 100 để tí mình nhân 100 để ra giây*/

    /* nói lại cách tính system clock thường = 72Mhz <-> 72_000_000
    -> 72_000_000 / 1000 = 72_000 mà Systick sẽ đếm từ 72_000 đến 0, mỗi xung mất 1/72_000_000 giây (tức là mỗi lần đếm sẽ đếm từng đây và sẽ đếm đến 72_000 lần rồi dùng lại) -> 72_000 * 1/72_000_000 = 0.001s = 1ms (logic thông thường thoi).
    */

    __enable_irq(); // enable clock cho system tick

}

void SysTick_Handler(void){ // hàm đã tồn tại từ trước trong core
    ++tick;
}
uint32_t getTick(void){
    __disable_irq();
    _tick = tick;
    __enable_irq();

    return _tick;
}
void DelayS(uint32_t seconds)
    seconds *= 100;
    uint32_t temp = getTick();
    while((getTick()-temp)<seconds){}/* hàm này nghĩa là ví dụ nhiều lúc getTick nó sẽ có giá trị như 5ms chẳng hạn và seconds kia là 100ms và ta sử dụng while getTick()<seconds vậy là sai ròi bởi nó sẽ đếm từ 5ms đến 100ms không đủ. 
    vậy nên biên temp được tạo ra để lưu giá trị của getTick hiện tại ví dụ là 25ms chẳng hạn: temp = 25ms, trong khi đóa getTick() vẫn liên tục đếm nhưng lần này nó đếm đến 100ms thì nó vẫn phải tiếp tục vì 100 - 25 vẫn nhỏ hơn 100 -> getTick() phải đếm đến 125ms -> và 125 - 25 = 100 hàm delay sẽ đúng.
    */
}
void blueOn(void){
    GPIO->ODR |= BLUE;
}
void blueOff(void){
    GPIO->ODR &= ~BLUE;
}
void OrangeOn(void){
    GPIO->ODR |= ORANGE;
}
void OrangeOff(void){
    GPIO->ODR &= ~ORANGE;
}

int orange_main(void){
    while(1){
        OrangeOn();
        DelayS(1);
        OrangeOff();
        DelayS(1);
    }
}

int blue_main(void){
    while(1){
        blueOn();
        DelayS(1);
        blueOff();
        DelayS(1);
    }
}

```


</details>

<details><summary><h1> 3. Introduction to Real-time Operating Systems </summary></h1>

- Về cơ bản như ta được biết thì hệ điều hành sẽ quản lý tài nguyên, nó sẽ điều khiển phần cứng và lên lịch các tác vụ, bằng cách nó sẽ phần bổ tài nguyên phần cứng cho các tác vụ nhất định. Vậy RTOS là gì ? Như ta thấy Real-time tức là thời gian thực tức là ta sẽ rằng buộc hay đảm bảo về mặt thời gian đối với các tác vụ.
VD: ví dụ như các chương trình trên window chăngr hạn nếu ta không tắt thì chương trình cũm sẽ không bao h tắt kể cả lỗi, và nếu nó sẽ đứng yên thế luôn. Còn với Real-Time thì 1 một chương trình sẽ chạy trong 1 time nhất định rùi chạy đến chương trình khác.
- Có 2 đặc điểm chính của RTOS là:
+ Thời gian(Deadline): Để tính toán xem liệu rtos có đưa ra kết quả đúng trong 1 thời gian nhất định không, kiểu chạy càng lâu thì không biết nó có sẽ đảm bảo về mặt time hay không ?
+ Độ tin cậy(Reliability): Ước tính độ tin cậy liệu rtos đang chạy ổn định và theo đúng với phản hồi được đảm bảo không ?

</details>

<details><summary><h1> 4. Software flow </h1></summary>
Watch video. Nói chung nó nói về flow của 1 chương trình.

- Polling: tức là cho hết các chức năng vào hàm main xong while để chờ sự kiện xảy ra -> làm như vậy rất dễ bị delay chương trình, delay các chức năng ... rất nhiều vấn đề

- IRQ: ổn hơn polling, các chức năng chỉ xuất hiện khi xảy ra ngắt, nhưng mà dễ bị chết chương trình trong IRQ 

- RTOS: quản lí bằng các task chạy // với nhau -> có trạng thái ngủ nghỉ và các task cũm được bật lên nếu cần thiết -> siêu tối ưu :))

</details>

# 5. The Stack
Nên xem video nha khá hay. Giới thiệu về stack, vị trí của Stack, ncl ôn qua về Stack :)) khá hay nếu mình không nhớ

<details><summary><h1> 6. Overview of Cortex-M OS Support Feature </summary></h1>

- Trong CPU có rất nhiều thành phần như NVIC, BUS Interface ... nhưng ta chú ý tới 2 bộ phận chính là Control Unit(CU) và Arithmetic Logic Unit(ALU) trong ALU sẽ có các thanh ghi cứng để tính toán các dữ liệu
    + Thanh ghi (Registers): R0 - R15 với R13 là SP(Stack Pointer) có MSP và PSP, R14 LR (Link Register), R15 Program Counter (PC)
    + Ngoài ra còn 1 số thanh ghi đặc biệt(special registers) không nằm trong register bank, nos bao gồm PSR (x3 loại) - trạng thái chương trình và ngắt, PRIMASK - chặn toàn bộ ngắt(trừ NMI), FAULTMASK - chặn luôn cả fault, BASEPRI  - Ưu tiên mức ngắt tối thiểu được phép, CONTROL - đổi chế độ Thread/Handle mode, stack pointer. Và các thanh ghi này mình không thể dùng lệnh load thông thường mà phải dùng câu lệnh assembly cho các thanh ghi này (MSR và MRS instruction). Còn vị trí của thanh ghi này nó sẽ nằm cạnh register bank. Giờ ta sẽ nói về thanh ghi PSR - có 3 loại là sao? Nghĩa là trong thanh ghi 32 bit ấy chia làm 3 phần Application PSR (bit 26-bit 31), Interrupt PSR (bit 0 - bit 8), Execution PSR (có thể là các bit còn lại) về chức năng của từng bit đọc kĩ hơn phần 2.3.2 trong document cortex_m3.

- Operation Mode: trong arm core có 2 modes và có 2 trạng thái operation, cũm như là processor có 2 trạng thái truy cập là privileged và unprivileged(user) access level. 

    + Thì với privileged thì ta được phép truy cập vào toàn bộ hệ thống, còn unprivileged thì khả năng truy cập vào các thanh ghi bị giới hạn như không truy cập được vào các vùng nhớ mà mpu không cho phép, hoặc các câu lệnh hệ thống đặc biệt (như thay đổi vector table), hoặc không chuyển đổi được các chế độ (thread mode, handle mode).

    + Tiếp theo thì arm cortex sẽ có 2 operation state là thumb state và debug state, với thumb state thì tức là nó sẽ là chương trình bình thường khi chạy 16 bit hoặc 32bit (halfword align Thumb and Thumb-2 instruction). Còn đối với debug state thì cta sẽ dùng chương trình lại và chạy chế độ debug thoi. Cụ thể thường thì ta ở trạng thái thumb state tạo 1 cái debug request và nó sẽ nhảy vào debug state khi debug xong sẽ trở lại thumb state.

    + Tiếp theo trong thumb state có 2 operation mode là thread mode và handle mode. Nôm na thì thread mode sẽ là khi chương trình trong hàm main, còn handle mode là khi chúng ta vaò trong hàm ngắt. Với handle mode thì sẽ luôn sử dụng privileged access, còn thread mode sẽ có thể vừa privileged access hoặc unprivileged access. Việc có những mode, state và các quyền truy cập này nhằm việc phát triển firmware tách biệt với phần application. Tức là phần OS sẽ có thể priviledge access, có đầy đủ khả năng truy cập vào thanh ghi, còn với người dùng user lớp application sẽ chỉ truy cập được vào unpriviledge access bằng việc đó thì khi application bị lỗi thì phần OS vẫn có thể chạy bình thường mà không bị ảnh hưởng.

- The Shadow Stack Pointer: tức là một chương trình như ta được biết sẽ có 2 stack pointer và 2 stack frame đó là main stack pointer và process stack pointer. Và main stack pointer thường hay sử dụng cho os Kernel và interrupt vì handle mode luôn sử dụng main stack pointer (hay sẽ luôn sử dụng privilegde access), còn với process stack pointer sẽ phù hợp sự dụng với các tasks. Nhưng với các chuong trình đơn giản ta hay sử dụng các task trong thread mode (tức là chương trình main) sử dụng với main stack pointer, nên là để tối ưu với mục đích sử dụng, phân chia rõ chương trình OS và Task, ta sẽ thay đổi main stack pointer sang process stack pointer ở chế độ thread mode.
    + Để chuyển chế độ từ main stack pointer sang process stack pointer ở thread mode ta phải truy cập vào thanh ghi CONTROL ở Special Register. Với bit số 1 luôn mặc định ở main stack pointer, và nên ta phải ghi thêm 1 để sang chế độ process stack pointer. Và với các Special Register ta phải sử dụng các Instructions như MRS và MSR.

- SVC Exceptions: đầu tiên ta sẽ dựa theo bài giảng thì SVC allows application tasks to gain system level privilege và allows application task to be developed independently of the OS. Ý đầu tiên muốn nói SVC sẽ cho phép application (thread mode - unpriviledge) có thể đạt được gọi là các quyền truy cập hệ thống, tức là có thể truy cập sâu hơn vào thanh ghi các thứ, đấy là khi SVC cho phép chuyển đổi sang priviledge. Ý thứ 2 muốn nói là cho phép application có thể gọi các quyền bên dưới OS mà không cần biết địa chỉ của nó như nào, để nói kĩ hơn ta sẽ trình bày bên dưới
    + Khi ta gọi 1 SVC #number thì nó sẽ nhảy vào SVC handler và cái SVC handler này sẽ do người khác viết và tầng application trên thực tế ở đây chỉ gọi cái SVC #number kia thoi. Nên do đó việc được phép chuyển đổi thành priviledge thông qua CONTROL register cũm phải phụ thuộc vào người viết SVC_Handler có cho phép bạn chuyển đổi hay không, chứ không phải cứ gọi là được.
    + Okee nói về cách thức hoạt động và vì sao dùng SVC ? Thì cơ bản khi mình đang ở chế độ thread mode và unpriviledge thì với chế độ này ta không được phép truy cập vào thanh ghi CONTROL register. Nên ta phải chuyển nó về chế độ priviledge và SVC handler chính là 1 cái Interrupt hay Exception. Tức là khi chúng ta gọi SVC #number là nó sẽ tạo ra 1 Exception. Nó khác với các Interrupt hay Exception khác ở chỗ là nó có thể gọi ra ngắt thay vì phải chờ như Timer hay chờ 1 sự kiện như ngắt GPIO. Vậy nó có thể gọi những gì và khi nào nhờ được nó? Ví dụ khi ta không dám hoặc không thể tự ghi vào RCC để thay đổi clock thì SVC đổi giúp, hoặc không thể reset hệ thống trực tiếp -> gọi SVC yêu cầu system reset, hay cần truy cập vùng flash đã bảo vệ -> gọi SVC để hệ thống kiểm tra và cho phép ... 

- Coding Creating SVC Services: 

    + SVC_Handler() -> Run SVC service -> Determine SVC number -> Read PC register -> Read Link Register: Có thể nói là nó sẽ thực hiện như sau khi gọi lệnh SVC, thì có thể nói SVC service nó sẽ chuyển từ thread mode sang handler mode. Tiếp theo determine SVC number thì chính là đọc cái number mà mình truyền cho SVC và lấy số đó để quyết định được nummber mà mình phải thực thi. Ngoài ra còn đọc PC register - dùng để đọc vị trí khi thoát ra khỏi SVC_handler thoi thì theo ông ấy giải thích trong video khá là khó hiểu, khi mà masking out unwanted bits - cái này có nghĩa là ta đều biết là chương trình ta đều chạy theo số chẵn hay aligned là nhảy 4byte hoặc 2 byte (32bit hoặc 16bit), nhưng cta cũm có cái thumb instruction set tức là các địa chỉ sẽ cộng thêm 1 để nói rằng nó đang ở chế độ thumb (vd: 0x08000201) vì vậy ông ấy nói mask unwanted bits tức là clear cái bit 0 từ 1 thành 0 để lấy cái địa chỉ thật. Ngoài ra ông ấy còn bảo we want to one that has the useful information, tức là ông ấy muốn lấy cái địa chỉ PC để lấy 1 cái thông tin gì đó nữa - thì nó chính xác dùng để -2 byte để đọc opcode của lệnh SVC để lấy cái number kia kìa, vậy tại sao lại là 2byte vì cái con trỏ PC nó nhảy sang dòng lệnh tiếp rồi, nên phải trừ đi 16bit để quay lại cái địa chỉ nó có dữ liệu, SVC là 1 lệnh 16bit trong thumb instruction set. Rồi từ opcode đó lấy ra 8 bits cuối sẽ lấy được SVC number (SVC #0x25 tương đương với opcode = 0xDF25) với DF là mã lệnh của SVC và 25 kia là tham số truyền vào thoi. Tiếp theo read link register là ta sẽ đọc để trả về thread mode hay handler mode cũm như dùng MSP hay PSP, Stack Frame tiêu chuẩn hay extended và cái giá trị này sẽ khá đặc biệt như 0xFFFFFF09. Và làm sao để biết là ta trả về PSP hay MSP thì ta sẽ đọc Link Register(EXC_RETURN) và để í tới bit số 3 nếu bit[2] = 0 return to process stack, bit[2] = 1 return main stack pointer.

    ```C
        oke thì giải thích thêm về SVC kia. Thì như ta được biết thì muốn chuyển sang tới SVC handler thì ta phải gọi SVC #5 ra. Tức là bắt buộc phải gọi ra mới nhảy vào ngắt SVC. Thì khi nhảy vào ngày nó bắt đầu quá trình Stacking -> lúc này mình sẽ nắm được toàn bộ register bank gồm PC, LR , R0,.. (nó được lưu ở 1 biến nào đó do mình tạo như orange_stack ấy) của chương trình trước khi vào hàm ngắt SVC.
        -> đến lúc này ta biết được con trỏ PC của nó trước khi vào hàm như nào.
        0x08001000: SVC #5
        Ví dụ địa chỉ PC trước khi vào hàm sẽ là 0x08001002(là câu lệnh tiếp theo trong hàm main nhớ) thì như ta được biết con trỏ PC nó sẽ trỏ tới địa chỉ tiếp theo thì cái địa chỉ 0x08001002 là địa chỉ tiếp theo nó nhảy đến. Còn địa chỉ hiện tại chính là 0x08001000 và chính cái địa chỉ hiện tại này là nơi mình gọi SVC #5. Đó thì cái địa chỉ hiện tại này sẽ lưu SVC number.
        Vậy nên khi vào hàm SVC handler hay đã vào SVC ngắt, thì ta muốn check SVC number là bao nhiêu thì ta phải vào tìm thông tin PC đã lưu, thì PC này chính là địa chỉ tiếp theo sau khi thoát khỏi hàm mình sẽ vào chính là địa chỉ 0x08001002. Vậy muốn lấy SVC number thì ta phải giảm đi 2 byte tức là đến địa chỉ 0x08001000 và trỏ vào đấy để lấy giá trị SVC #5. Thì cái SVC #5 này nó sẽ được biến đổi sang 1 cái mã opcode có dạng 0xDF05. Tức là giá trị thực tế mà thanh ghi 0x08001000 lưu là 0xDF05. Đó thì mình sẽ lấy 8 bit cuối của cái opcode này -> sẽ tìm được giá trị SVC number.

        Tiếp theo thì tại sao lại trừ 2 byte là 16 bit thì SVC nó là thumb intruction set -> nó sẽ nhảy 2 byte tức là từ 0x08001000 đến 0x08001002 -> ta sẽ trừ 2 byte để đến đúng vị trí con trỏ hiện tại. Thì chương trình của mình có 1 số câu lệnh sẽ là thumb2 tức là nhảy 4 byte ví dụ 0x08001000 đến 0x08001004. Nma SVC của mình chỉ nhảy 2 byte thoi -> -2 byte
        
        svc_number = ((char*)src_args[6])[-2];
        thì câu lệnh này src_args[0] sẽ lưu R0 -> src_args[6] sẽ lấy giá trị là con trỏ pc tức là src_args[6] = 0x08001002 lúc này giá trị nó chỉ là 1 con số. Thì đến đây ta ép số 0x08001002 thành con trỏ char* í muốn nói giờ con số này là 1 con trỏ. Và khi ta sử dụng 0x08001002[-1] hay 0x08001002[2] tức là sử dụng nó như mảng ấy :)) thì nó sẽ truy cập vào giá trị địa chỉ p[-2]  ≡  *(p - 2) đây giống như này. Còn nếu ++ hay -- thì đơn giản là tăng địa chỉ hoặc giảm địa chỉ chứ không thể truy cập, truy cập thẳng chỉ có dùng 0x08001002[-1] kiểu này thoi
        Mà (char*)0x08001002++; như này là rvalue chỉ giá trị tạm thời → chỉ để dùng, không gán được(vì giá trị tạm thời này không có chỗ đứng trong bộ nhớ) không thể thay đổi nếu muốn ++ địa chỉ phải char *p = (char*)0x08001002; và p++; là địa chỉ tăng p = 0x08001003;

        Khái niệm: 
        l-value = thứ có chỗ đứng trong bộ nhớ → có thể gán / thay đổi
        r-value = giá trị tạm thời → chỉ để dùng, không gán được (tức là nó không có chỗ đứng trong bộ nhớ thì không thể thay đổi)
    ```

    + Vậy SVC sẽ làm được gì trong RTOS? Thì như ta được biết thread sẽ unpriviledge nên sẽ không thể làm trực tiếp với kernel nên SVC sẽ gọi kernel để thực hiện 1 số task vụ như delay() hoặc tạo task.... đại loại là như vậy.

```C

/*đây là câu lệnh trong arm compiler và nó không dùng được trong C, hay nó được gọi là 1 function atrribute, nó thông báo với compiler là phải xử lý đặc biệt với nó */

int __svc(0x00) svc_service_add(int x,int y);
/*__svc(0x00) sẽ tương đương việc gọi SVC 0x00
__asm volatile ("svc #0x00"); tương đương với câu lệnh như này trong C
và khi gọi như này nó sẽ thực thi như sau:
khi ta gọi svc_service_add(5,7); chẳng hạn thì chương chính sẽ thực hiện SVC 0x00 và nó sẽ nhảy vào SVC_handler chung và trong đấy nó sẽ đọc number 0x00 và nhảy vào điều kiện đó để thực thi câu lệnh liên quan
*/

int __svc(0x01) svc_service_sub(int x, int y);
int __svc(0x02) svc_service_mul(int x, int y);
int __svc(0x03) svc_service_div(int x, int y);

int x,y,z;
int main(){
    x = 1;
    y = 5;
    z = svc_service_add(x,y);

    x = 9;
    y = 2;
    z = svc_service_sub(x,y);

    x = 3;
    y = 4;
    z = svc_service_mul(x,y);

    x = 12, y = 6;
    z = svc_service_div(x,y);
}

__asm void SVC_Handler(void){
    /*TST Rn , Operand2
    Ý nghĩa là thực hiện phép AND giữa Rn và Operand2, nhưng không lưu kết quả, thay vào đó, nó chỉ cập nhật các cờ trong thanh ghi trạng thái (Zero flag, Negative flag nằm ở trong thanh ghi PSRx)
    Vì TST này dùng để trả về trạng thái 0 và 1 nên nó sẽ được lưu ở zero flag, còn nếu nó âm nó sẽ được lưu ở negative.
    )
    */
    TST LR, #4 // 0b 0100 đang đi xem bit số 3 của thanh ghi LR là bit gì để xem là chương trình sẽ trả về process stack hay main stack.

    /*ITE - If then else đây là 1 block điều kiện
    EQ = Equal -> nó sẽ vào check zero flag
    ITE EQ tức là EQ = 1 thực hiện câu lệnh ngay bên dưới
    Còn EQ = 0 thực hiện câu lệnh dưới nữa
    */
    ITE EQ
    MRSEQ R0, MSP // nếu đúng thì lấy giá trị MSP vào R0, chú ý là nó ghi vào CPU register(register lõi vật lý)
    MRSNE R0, PSP // nếu sai thì lấy giá trị PSP vào R0
    B       ___cpp(SVC_Handler_C) // B - Branch là nhảy tới 1 địa chỉ hay lable không cần điều kiện giống như goto.
}
/*với *src_args nạp từ thanh ghi R0, src_args trỏ tới stack frame*/
void  SVC_Handler_C(unsigned int *src_args){
    unsigned int svc_number;
    /* cái này nghĩa là nó sẽ lấy 2byte thấp của src_args[6] đang trỏ vào pc trong Stack Pointer giải thích bên trên phần code ròi*/
    svc_number = ((char*)src_args[6])[-2];
    switch(svc_number){
        case 0:
            /*giải thích chỗ này thì cũm dễ hiểu thoi, thì ghi quá trình Stacking tức là quá trình khi m vào 1 cái ngắt hay 1 cái function. Nó sẽ lưu tất cả các thông số R0 R1 ... PC LR vào Stack Frame. Thì khi ở hàm main ta gọi 1 cái SVC ví dụ gọi svc_service_add(x,y); Thì x,y là parameter của hàm đk. Thì theo mặt vật lý thì parameter sẽ được ghi vào R0 và R1 ..., nếu có nhiều parameter hơn thì nó sẽ ghi thêm vào R2,R3 ... tức là khi m gọi svc_service_add(x,y), lúc này ở CPU Register Core R0 = x, R1 = y. Sau khi nhảy vào hàm SVC_Handler, trước đó nó sẽ có quá trình stacking, thì lúc này Stack Frame cũng đang lưu R0 = x, R1 = y. Và cái quan trọng này con trỏ src_args là trỏ tới stack frame, chứ không phải trỏ tới CPU Register. Vậy lên trong SVC_Handler, ta dùng MRSEQ R0, MSP thì lệnh này nó ghi trực tiếp vào CPU register, chứ nó không ghi vào stack frame. -> 2 hàm tác động vào 2 vùng nhớ khác nhau.

            Đó thì giờ ta đã lưu thông tin PC R0 R1 trước khi xảy ra ngắt SVC vào stack frame ròi (tức là PC = địa chỉ tiếp theo sau SVC ở hàm main, R0 = x, R1 = y, LR = quay về main() dựa vào PC) và ta gọi đây là stack_frame_1 đi. Đó bây h ta vào hàm SVC_handler lúc này stack_frame_1 đã được lưu vào trong stack_frame và MSP đang trỏ tới R0 của stack_frame_1. Thì khi ta dùng B       ___cpp(SVC_Handler_C) lúc này nó sẽ nhảy sang 1 hàm mới, mà nó lại thức hiện quá trình stacking -> MSP sẽ dịch chuyển. Việc MSP dịch chuyển sẽ khiến ta mất thông tin giá trị stack_frame_1, vì nó sẽ nhảy tới 1 frame khác, không còn là stack_frame_1 nữa.
            Vậy nên ta phải sử dụng MRSEQ R0, MSP để lưu MSP đang trỏ tới stack_frame_1 vào R0. Thì khi ta B       ___cpp(SVC_Handler_C), tức là chuẩn bị nhảy sang hàm SVC_Handler_C, thì stack frame mới sẽ có giá trị (PC trỏ tới SVC_Handler_C, R0 = MSP(MSP này đang trỏ vào địa chỉ stack_frame_1), LR quay trở về SVC_Handler()) thì ta gọi đây là stack_frame_2. Đó thì như ta được biết phần cứng của ARM, nó sẽ coi R0 là tham số đầu tiên của hàm, R1 sẽ là tham số thứ 2, nó giống R0 = x, R1 = y. Thì ở đây R0 của stack_frame_2 là = địa chỉ của stack_frame_1 và với việc src_args là tham số đầu tiên của hàm SVC_Handler_C -> src_args = địa chỉ của stack_frame_1(tức là PC = địa chỉ tiếp theo sau SVC ở hàm main, R0 = x, R1 = y) lúc này ta chỉ cần lấy ra để dùng.

            Oke nói thêm 1 số kiến thức này thì cái LR nó có cái mã return ấy. Thì nó không return cả cái địa chỉ muốn trả về đou. Mà nó sẽ return kiểu: hàm nó chuẩn bị trả về là thread hay handler mode, còn địa chỉ trả về nằm ở PC trong stack frame. Vậy nên svc_number = ((char*)src_args[6])[-2]; ta phải trừ đi -2 byte đó. PC lưu vào là lưu địa chỉ tiếp theo trong main chứ không phải địa chỉ chạy SVC #0. Phải nhớ là khi nó chạy SVC #00, PC nó đang lưu dịa chỉ tiếp theo ở main(), và Stacking xảy ra nó sẽ lưu hết tất cả các giá trị của Register Bank như R0,R1,R2,R12, xong lưu tới PC, LR và xPRS hiện tại(hiện tại PC đang trỏ tới địa chỉ tiếp theo ròiii). Đó thì lưu hết vào stack frame và mình chỉ lấy ra bằng các quy trình trên thoi
            */
            src_args[0] = src_args[0] + src_args[1];
            break;
        case 1:
            src_args[0] = src_args[0] - src_args[1];
            break;
        case 2:
            src_args[0] = src_args[0] * src_args[1];
            break;
        case 3:
            src_args[0] = src_args[0] / src_args[1];
            break;
        default:
            break;
    }
}

```

- PendSV Exception: người ta có nói Minimize latency experienced my Interrupt Service Routines nghĩa là PenSV sẽ giúp giảm thiểu độ trễ mà các hàm ngắt phải chịu. Tức là như nào thì cái PenSV exception này sẽ có mức độ ưu tiên thấp nhất và nó sẽ giữ cái khả năng chuyển đổi task, tức là muốn chuyển task thì vào PenSV, và khi 1 ngắt xảy ra thì nó cứ làm mấy cái ngắt kia trước rồi chuyển Task sau. Thì việc này sẽ không dẫn tới tình trạng delay ngắt khi ví dụ đang thực hiện ngắt nào đó, tự dưng đi chuyển sang làm task khác thì không ổn vì sẽ gây delay ngắt, nên là ta cứ để thực hiện các ngắt trước rồi chuyển đổi sang task khác sau. (cứ mà tài liệu Arm-Cortex M3, M4 trang 336 là thấy rõ hậu quả của việc không PendSV :)))
    + Và thằng PenSV sẽ nắm giữ context switching là cái quá trình mình lưu và đẩy dữ liệu stack frame lên stack á. Nói ở phần bên trên ròi, ở phần code đầu tiên. ở ví dụ led_red hay gif gì đấy.

VD về quy trình lần lượt priority sẽ là OS(Systick) - Interrupt - SVC - PendSV và Thread sẽ như sau: đầu tiên Task A sẽ được thực thi (thread), xong ví dụ ta set là cứ 1ms sẽ nhảy vào Systick, thì sau khi thực hiện 1ms ở Task A, thì mình sẽ nhảy vào Systick Handler, và ở trong Systick Handler đó mình sẽ kích hoạt PendSV thông qua thanh ghi(thông thường là ta sẽ dùng SVC để gọi ra PendSV, nma ở đây chắc là :)) chỉnh bằng tay qua debug nên mới bảo thông qua thanh ghi), và sau đó mình sẽ nhảy vào PendSV để thực thi qua trình context switching. Và sau đó mình sẽ chuyển qua Task B và ví dụ ở đây sẽ có 1 ngắt thì đang thưc hiện giữa chương trình Task B chẳng hạn thì có 1 cái ngắt xảy ra, đương nhiên theo kiến trúc máy tính nó sẽ lưu hết dữ liệu stack frame của task B đang làm dở, và nhảy vào hàm ngắt, trong hàm ngắt này đang làm dở chẳng hạn thì thời gian Systick đủ 1ms thì nó sẽ lại nhảy vào Systick và ở trong đây mình lại kích hoạt ngắt PendSV nhưng mà  ngắt PenSV có mức độ ưu tiên thấp nên khi thực hiện Systick xong nó sẽ lại nhảy về ngắt, và khi ngắt thực thi xong mới thực hiện ngắt PenSV và đồng thời cũm nhận được dữ liệu Stack Frame của Task B như theo cấu trúc máy tính thui, thì trong PenSV lại xử lý quá trình context switching. Đến tiếp Task C thì mình sẽ không sử dụng ngắt Systick cho task này chẳng hạn mà mình sẽ dùng SVC để gọi PendSV, thì quá trình sẽ thực hiện như sau thì khi ta làm task C có thể là thực hiện xong rồi, mình sẽ gọi 1 cái SVC #number nhằm vào thực hiện PendSV và giờ lại vào PendSV thực thi như bthg thoi.

Lưu ý quan trọng
```text
Thì cái IRQ nó có 1 lưu í đó là nó không nên thực hiện các task vụ phức tạp trong đó, tại chương trình có thể bị chết trong ngắt luôn, hoặc nó diễn ra quá lâu thì mất cái tính chất RTOS của chương trình. Vì những task của RTOS không thể có quyền ưu tiên cao hơn ngắt được.
Vậy nên trong IRQ cta chỉ nên thực hiện các task vụ không phức tạp, nhanh. Và thông thường trong theo cách thiết kế chuẩn thì IRQ chỉ nên dùng để đánh thức task, không nên gọi task, tại gọi thì nó vẫn là ở trong ngắt thoi, còn đánh thức là mình chỉ làm mỗi hành động đánh thức nó dậy, ròi tí nữa thoát ngắt, task mà mình vừa đánh thức sẽ hoạt động cùng với các task có sẵn trong scheduler.

Ví dụ có 3 task A, B, C. (Task C sẽ là task ẩn nó chỉ được chạy khi IRQ đánh thức nó, còn đâu trên thực tế chỉ có 2 task chạy A,)
Bình thường scheduler chỉ chọn task READY (A, B) để chạy,task C đang BLOCKED / SLEEP.

Khi IRQ xảy ra, ISR chỉ đánh thức task C → chuyển state C từ BLOCKED → READY (chuyển state thường rất nhanh 1 câu lệnh là xong)

Sau khi IRQ kết thúc, scheduler chạy lại.(kết thúc ngắt là nó thoát ngắt luôn ấy, về hàm main() của mình ròi).
Vì task C có priority cao hơn A và B, nên task C được chạy trước. Đó thì tức là ta nên thiết kễ những cái task được đánh thức nên có priority cao hơn các task mà chạy từ đầu, như thế nó mới chuẩn kiến trúc ta muốn.
```

- Exclusive access instructions: từ này vốn dĩ có liên quan đến việc sử dụng tài nguyên chung 1 cách an toàn, đặc biệt đối với các hệ thống đã luồng multithread hay multicore, tức là trong các lõi Arm nó sẽ có cái instruction là LDREX và STREX giúp ta atomic tức là giam 1 cái biến vào để tránh bị nhiều task khác cùng dùng.
    + Ví dụ ta có 1 biến count global và có 2 task. Task 1 ghi dữ liệu count++ và làm số việc khác bên dưới, còn Task 2 sẽ là lấy dữ liệu từ biến count. Giả sử ta đang định ++ biến count ở task 1 thì chuyển task2 thì nhìn count++ có vẻ đơn giản nhưng để ++ được thì nó phải trải qua 3 instruction LOAD, ghi giá trị và lưu vào RAM. Giả sử như trên nó vẫn mới đang ở bước LOAD giá trị vào thanh ghi mà ta lỡ sang Task2 và lấy vào biến count tiếp thì 1 là nó lỗi compiler, 2 là nếu lấy được biến count thì cũm là giá trị sai vì ở Task1 biến count này mới lưu giá trị vào thanh ghi chứ chưa cộng lên và trả lại giá trị vào RAM, nên Task 2 đọc RAM lấy giá trị sẽ sai. Thì để tránh sai thì ở C++ hay có kiểu mutex ấy nó sẽ giam các biến hoặc đoạn code mình không cho phép task vụ khác dùng. Còn đối vi điều khiển này ta có thể dùng disable ngắt cái đoạn mình cần làm và lại enable nó lên là chả có gì can thiệp được nhưng nó có thể gây trễ tiến trình ngắt, ngoài ra còn cái trong lõi Arm nó sẽ có instruction LDREX và STREX để thực hiện điều đó.
    + Vậy thì LDREX và STREX là gì? LDREX(Load Exclusive) nghĩa là đọc giá trị từ 1 địa chỉ trong RAM và lưu giá trị đó vào thanh ghi (R0 chẳng hạn) và đồng thời CPU đánh dấu nó là exclusive hoặc là đang được theo dõi. Còn STREX(Store Exclusive) nghĩa là ghi giá trị vào cùng địa chỉ và nó chỉ ghi khi chưa ai chạm vào nó từ lần LDREX. Tức là như nào tức là sau quá trình mình sử dụng LDREX xong thì mình thực hiện tăng giá trị đó lên chẳng hạn, cái STREX sẽ kiểm tra xem có ai khác can thiệp vào địa chỉ đó không, nếu không ai chạm vào thì ghi thành công trả về 0 còn nếu có ai khác ghi vào giữa chừng nghĩa là thất bại sẽ trả về giá trị khác 0. Thì như ta thấy điểm yếu của cái này cũm khá lớn vì nó chỉ kiểm tra xem có ai động vào địa chỉ đấy để trả kết quả sai thoi, nếu trường hợp luôn có 1 task B nào đóa luôn nhảy vào để thay đổi giá trị cái địa chỉ count đúng lúc quá trình mình count++ bên trong LDREX/STREX ở Task A. Thì khi trả về task A cái STREX sẽ luôn trả về giá trị khác 0 tức là việc count++ đã không hoàn tất. Điều đó dẫn tới việc count++ có thể chả bao h được cộng lên, còn việc đọc thì vẫn được nha :)) íi là đọc dell bị ảnh hưởng tới STREX và trả về 0, tức là chỉ thay đổi giá trị bên trong thì lỗi, còn đọc thì dell lỗi. Nhưng vấn đề là :)) dcm ngắt với context switching cũm gây ra lỗi. Vậy giải quyêt như nào :)) thì ta disable và enable ngắt đoạn code đó :)) nghe chán vch.


- Systick Interrupt: ncl dùng để tạo thời gian và nó nằm trong core, 1 bộ đếm của core. Gồm 24bit down counter. Có 4 register quan trọng là:
    + Control and Status Register: use enable and disable Systick.
    + Reload Value Register: dùng để nạp giá trị đếm cho Systick hay period.
    + Curent Value Register: dùng để clear giá trị (là khi mình làm giá trị bất kỳ vào thanh ghi này thì sẽ reset giá trị đếm về 0)
    + Priority Register: dùng để thiết lập priority thoi.
    + Step code: Disable -> Set Period -> Clear Initital Value -> Set SysTick priority -> Enable Systick -> Set clock source -> Enable Interrupt.

</details>

<details><summary><h1> 7.BOOT Sequence </summary></h1>

- Là quá trình mình ấn reset trên board, lúc ấy quá trình boot sequence sẽ xuất hiện. Thì như trong video người ta có nói là nó sẽ reset lại tất cả các giá trị trong register. Tiếp theo thì processor sẽ quyết định boot mode, sau quá trình chọn boot mode thì tùy vào boot mode thì cơ bản nó sẽ lấy MSP từ địa chỉ 0x00000000, tiếp theo nó sẽ lấy PC từ địa chỉ 0x00000004. Cái mới ở trong video thì ta có thể thấy là địa chỉ ở 0x00000004 là địa chỉ PC, thì ta vẫn nghĩ nó sẽ nhảy vào hàm main(), nhưng thực tế nó sẽ nhảy vào 1 cái hàm reset_handler() trước và trong cái hàm reset_handler đó sẽ chứa hàm main() và sau đó mình mới nhảy tới main(), ngoài việc gọi hàm main() ra thì trong reset handler nó có thể khởi tạo 1 số quá trình như đẩy dữ liệu từ flash lên RAM dựa vào file startup hoặc linker chẳng hạn. Thêm nữa là như trong video thì cái địa chỉ ở 0x000000004 thì giá trị bên trong nó lẻ thì là do chễ độ thumb mode hoặc arm mode, thì thumb sẽ lẻ, nên là muốn nhảy tới địa chỉ đó thì mình phải làm nó chẵn đã, không là jump tới đó luôn bị lỗi đó.
    + Để rõ hơn mình sẽ xem lại video ở trên youtube về quá trình Booting rồi tổng hợp lại. Thì việc Boot nó sẽ dựa vào các Pin của Boot, thì với BOOT[1:0] với BOOT1 là 0 và BOOT0 = 0 thì nó sẽ boot vào main flash memory. Ngoài ra còn 1 số chế độ khác như boot vào system memory hoặc boot lên SRAM. 
    + Ngoài ra ta còn biết tới bảng Vector Table về cơ bản bảng này nó cơ bản sẽ bao gồm tất cả các ngắt, exception, các offset của ngắt, và còn có initial sp value và địa chỉ đầu tiên của Vector table. Và cái bảng Vector Table này có thể reallocate, tức là phân vùng cho nó ở 1 địa chỉ khác nhờ vào thanh ghi SCB_VTOR. Và cái bảng vector table này sẽ được ghi trong file startup
    + Nói thêm về file startup thì nó sẽ là file như cái tên là khởi tạo, tức là trước khi chạy vào main chúng ta sẽ chạy vào file này trước để khởi tạo các cái hàm exceptions và interrput thì tức là mình sẽ đki cái tên exception hay interrupt cho hệ thống. Ví dụ đơn giản như GPIO_Interrupt thì chỉ gọi tên là 1 cái hàm thì sao mà hệ thống biết được. Thif đương nhiên mình phải đki cho hệ thống cái tên đó, thì file startup nó sẽ là nơi mình đki tên và vector table thì chứa các hàm đó đó, vector table nó nằm ở mảng đầu tiên của file start up luôn. Ngoài ra cụ thể hơn thì ở cái hàm reset_handler kia mình sẽ gửi dữ liệu từ flash lên RAM. Và cái dữ liệu từ flash lên RAM này sẽ dựa vào linker script vì ta có phải copy toàn bộ tất cả các dữ liệu lên RAM đâu, mà ta chỉ copy các vùng cần thiết như .text, .data kiểu kiểu vậy và linker script sẽ là người phân bổ đó.
    + Về Linker script: nó sẽ phân bố ROM bắt đầu ở đâu, RAM bắt đầu từ đâu trên bộ nhớ, vùng .text code hay vùng .data sẽ ở đâu, ngoài ra còn 1 số các symbol nữa như _sdata,_edata cái này có thể xem sau. Thì cái Boot nó sẽ giúp như nào, thì cái boot nó sẽ quy định vùng nhớ bắt đầu ở đâu, còn linker script sẽ là người phân bổ code và dữ liệu, kiểu đâu là vùng .text, vùng .bss.
    + Vậy khái niệm bên trên xong rồi thì quá trình Boot chi tiết sẽ như nào? Thì code của mình được lưu ở đâu là do thằng linker quy định, và thông thường bảng vector table sẽ được allocate ở đầu của vùng flash. Thì ta được biết khi ta BOOT[0,0] mode tức là nó sẽ sử dụng Flash thì vùng này sẽ bắt đầu từ địa chỉ 0x80000000, và cái vùng 0x00000000 sẽ ánh xạ đến cái vùng 0x80000000 đấy, tức là 2 vùng này sẽ có dữ liệu giống nhau đơn giản là vùng 0x00000000 này copy của vùng 0x80000000 kia thoi. Thì khi mà Reset thì thằng VTOR sẽ luôn = 0, và xảy ra quá trình ánh xạ mà con chip khi reset nó sẽ luôn tìm tới địa chỉ 0x00000000 và nó sẽ khởi tạo các thông số dựa vào bằng vector table. Và trong bảng vector table có gì mình đã nói ở trên.

</details>

<details><summary><h1> 8.Introduction to Threads </summary></h1>

- Ncl thread nghĩa là thì ví dụ có nhiều task đó, thì mỗi task sẽ phải có 1 Stack Pointer riêng hay 1 cái Register bank riêng(hay có thể nói là có stack frame riêng)  thì mới chạy // được, nhưng như thế thì cần 4 core lận, tốn tài nguyên và không hiệu quả. Thì Thread chính là việc mình chạy 4 task trên 1 core duy nhất thoi. Và đặc điểm của Thread sẽ là không chạy kiểu frequently, kiểu tùy phụ thuộc vào mình thiết lập kiểu gì, có những thread lỗi hệ thống mới nhảy vào.

- Classification of thread (phân loại):
    + Timed Threads: Thì cái này sẽ nói về thời gian được định trong Thread như ta nói ở trên thì có những thread lỗi hệ thống chắc mới được vào, hoặc có những Thread chạy tuần tự 

    Sporadic Thread: Thì cái Sporadic Thread thường sẽ không có chu kỳ rõ ràng, chạy khi có sự kiện xảy ra, như ngắt do người dùng, hoặc lỗi hệ thống nhưng cái này nó sẽ bị giới hạn về tần suất thực thi. Tức là ví dụ m có 1 nút nhấn ngắt để nhảy vào 1 hàm Sporadic Thread chẳng hạn thì cái này nó sẽ giới hạn như 1s m mới được nhảy vào 1 lần. Tức là dưới 1s m nhấn thỏa mái nó cũm chả thực hiện lại. Giống như việc count++ thay vì ấn liên tục tăng liên tục thì 1s sau ấn mới có thể tăng.

    Aperiodic Thread: đối với Aperiodic Thread thì nó sẽ có thể thực hiện liên tục và thường xuyên nhưng mà nó vẫn cần 1 sự kiện như ngắt để nó xảy ra, kiểu nói nó thực hiện liên tục và thường xuyên vì m cứ gọi là nó thực hiện chả bị giới hạn về thời gian như Sporadic Thread, đấy cũm là điểm khác nhau.

    Periodic Thread: còn đối với Periodic Thread thì nó quá đơn giản, nó sẽ kiểu được fixed cứng 1 thời gian nhất định và cứ đến thời gian đó là chạy, như cứ 1s nhảy vào 1 lần chả cần yêu cầu về ngắt, chỉ cân yêu cầu về thời gian.

    + Event triggered Threads: Thì cái này nó nói về các sự kiện ngắt thoi như ngắt flag khi flag triggered chuyển đổi trạng thái 0->1 chẳng hạn thì nó sẽ chuyển đổi thread, và nó xảy ra trong flag nội bộ như cờ ngắt Systick chẳng hạn. Còn input-trigger là các dữ liệu bên ngoài vào như nút nhấn. Còn output-trigger được kích hoạt khi mình gửi dữ liệu hoặc bật tắt đèn thông qua output. (hay còn với thread là ta sẽ bật tắt thread theo í muốn)

    + Main Threads: thì nó được sử dụng khi ban đầu mình vào, mình khởi tạo các task ở trong main í, rồi sau khi chương trình RTOS mình biết có khác task nào thì nó mới chạy các task đó

- Some Key Term:
    + Non Real-Time: No guarantee of task execution (tức là chả có rằng buộc hay đảm bảo về mặt thời gian)
    + Real-time (Hard Real-Time): Bounded latency, guarantees execution. (độ trễ bị giới hạn, đảm bảo thực thi). Và nó sẽ giống cái quy trình PenSV ấy 
    + Soft Real-Time: Executes on priority basis (tức là nó cũm real-time nhưng dựa vào priority mà priority thấp thì chỉ có ăn cức thoi)
    + Latency: Execution delay
    + Periodic Thread: Runs at a fixed time interval.
    + Aperiodic Thread: Run frequently, runtime cannot be anticipated.
    + Sporadic Thread: Run infrequently or never.
    + Blocked state: waiting state.
    + Run state: meaning thread currently is executed.

</details>

<details><summary><h1> 9. Thread Control Block(TCB) </summary></h1>

- Thì nếu đọc sơ qua thì cơ bản là cái struct tcb{} này nó sẽ lưu các thông tin của cái Thread của nó (hay task của nó). Thì ví dụ khi nhảy vào PendSV tức là quá trình Context Switching diễn ra thì cái tcb ở Task A sẽ lưu các thông tin của nó lại, còn ở PendSV sẽ lấy tcb ở Task B để run, đó đơn giản vậy thoi. 
- Theo như trong video thì nó sẽ là 1 cái struct chứa những cái thông tin private của thread đó. Và nó sẽ bao gồm:
    + Bắt buộc phải có sẽ là: Pointer to stack và Pointer to the next thread.
    + Ngoài ra còn có: Variable to hold thread status, Variable to hold thread ID, Variable to hold thread Period, Variable to hold thread Burst time, Variable to hold thread Priority, ...

```C
struct tcb{
    uint32_t *stackPt;
    struct tcb *nextPt;
    uint32_t status;
    uint32_t Period;
    uint32_t burstTime;
}
```

**Implementing a Thread Control Block:**

- Về cơ bản nó sẽ là như này và mình phải giải thích tại sao lại có những dòng code này trong quá trình RTOS. Ví dụ dưới đây là 4 task
```C
// Giải thích code ở dưới phần code
#define NUM_OF_THREADS      4
#define STACKSIZE           100
struct tcb{
    int32_t *stackPt; 
    struct tcb *nextPt;
}

typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;
int32_t TCBs_Stack[NUM_OF_THREADS][STACKSIZE];
```
- Tự dưng thấy nó cũm đơn giản :)) int32_t *stackPt kia sẽ lưu giá trị của con trỏ stack pointer hiện tại của task của mình trước khi nó sang task khác (giá trị của con trỏ stack pointer sẽ trỏ tới stack frame mà ta biết stack frame sẽ là nơi lưu các thông tin trả về như PC, LR, R0 ... tức là int32_t *stackPt đang trỏ tới top stack frame của task A). Nói rõ hơn để đỡ lú là ta có Register Bank để thực hiện chương trình và Task A chẳng hạn sẽ có Stack Frame riêng, và khi muốn context switching sang Task B thì phải lưu cái thanh ghi hiện tại là cái Register Bank đấy vào trong Stack Frame gồm 7 cái thông số gì đóa đóa (R0 R1 R2 R12 PC LR xPSR). Đấy xong rồi cái biến int *stackPt kia sẽ lấy cái Stack Pointer vừa được lưu vào Stack Frame của Task A đó. Và làm sao mà Register Bank push vào được thì khi ta vào PendSV từ R0->xPSR nó sẽ tự push vào Stack Frame, và trong handler PendSV(nó sẽ vẫn ở trong PSP - theo chat GPT là vậy :)) còn nếu nó ở MSP thì nghỉ chịu chết dell biết push kiểu gì) mình sẽ phải tự push nốt R4->R11 (tự push bằng cách lấy từ register bank vào cái stack pointer hiện tại thoi) và đến R11 thì top Stack ở đó đó và stackPt = top Stack. 
Đó thì bên trên t cũm vừa nói thì việc lưu tcb này sẽ diễn ra trong PendSV. Đầu tiên các thanh R0 R1 R2 R12 PC LR xPSR sẽ tự động push vào stack frame, còn các thanh ghi R4->R11 ta sẽ tự tay push nó vào trong stack frame -> lúc này PSP đang trỏ tới R4. Thì đến lúc này stackPt = PSP, là stackPt đã lưu được top stack frame của task A xong. Tiếp theo nó bắt đầu thực hiện chuyển sang task B. Còn việc PendSV vẫn dùng PSP thì ta sẽ xem xét lại sau :)) theo t đọc lại MSP nó phải chạy bằng MSP, nhưng mà trong PendSV nó truy cập được vào thanh ghi mà nên nó dễ dàng lấy được PSP thoi
Code PendSV:
```s
MRS   R0, PSP        ; lấy PSP của task
STMDB R0!, {R4-R11}  ; save SW context
STR   R0, [TCB]      ; lưu PSP vào TCB

```


còn *nextPt sẽ trỏ tới địa chỉ của tcb Task khác, đơn giản là lấy địa chỉ thủ công thôi :)) 
VD: Muốn lấy stack của Task B để context switching
Đầu tiên ta lấy địa chỉ của tcb Task B: TaskA-> nextPt = &tcb_TaskB; (tcb này là global nên thích lấy lúc nào thì lấy)
Xong chta sẽ cho MSP hoặc PSP: PSP = TaskA->nextPt->stackPt vậy là lấy được stack của Task B thôi. Đương nhiên mấy code trong PendSV phải code bằng Assembly hết.

Còn int32_t TCBs_Stack[NUM_OF_THREADS][STACKSIZE]; chính là tạo vùng stack riêng cho từng thread đó :)). Đây chính là code ta cấp stack frame cho từng thread.
Thì cơ bản đây là 1 mảng 2 chiều liên tiếp
VD: NUM_OF_THREADS = 3 và STACKSIZE là 100 byte đi thì với thread 1 nó sẽ là kiểu TCBs_Stack[0][0] -> TCBs_Stack[0][1] -> ... -> TCBs_Stack[0][100]. Thì đây chính là stack frame và ta hoàn toàn có quyền truy cập vào từng địa chỉ 1 của stack frame :)). Thì tương tụ như thread 2 sẽ là TCBs_Stack[1][0] -> TCBs_Stack[1][100] ncl nó se liên tiếp nhau luôn

tcbType *currentPt; còn cái này thì như cái tên thì nó sẽ trỏ tới tcb hiện tại để mà chạy chương trình thoi.

</details>

<details><summary><h1> 10.The Scheduler and Scheduling Alogorithm </summary></h1>

***The Scheduler: Thread has 3 three state**
Trong 1 thời điểm chỉ có 1 thread hay 1 task được thực thi bởi processor. Và cái scheduler này sẽ do mình lập ra, cái độ ưu tiên cũm do mình lập ra giữa các task/thread. Gần tương tự như ngắt, nhưng khác chỗ là ngắt là phần cứng được fixed sẵn trong ISR, còn đây nó sẽ giống như các hàm bình thường và mình lập lịch ưu tiên cho các hàm này thoi. 

- Running: là task/thread mà processor đang thực thi.
- Ready: là những task/thread sẵn sàng chạy nhưng phải đợi task đang ở running chạy xong
- Blocked: là những task/thread đang chờ sự kiện ngắt xảy ra.

Các trạng thái chuyển đổi trong schedular:

- READY-RUNNING: trong quá trình Running, scheduler sẽ gắn next thread đối với các Ready Thread đang trong hàng đợi. Xảy ra khi task running hiện tại thực thi xong, và lấy Task đang đợi tiếp theo để thực thi.

- RUNNING-READY: Xảy ra khi task đang thực thi có độ ưu tiên có độ ưu tiên thấp hơn task trong hàng đợi. Nó sẽ bị chiếm đoạt bởi Task khác và nó phải trở lại Ready Thread để chờ Task thực thi cao hơn thực thi xong rồi nó mới được chạy.

- RUNNING-BLOCKED: Là task đang thực thi đã thực thi xong và vào Block để chờ 1 sự kiện như được gọi ra hoặc 1 ngắt xuất hiện.

- BLOCKED-RUNNING: Là khi 1 task xuất hiện và nó có priority cao hơn cả các task trong running và ready, nó sẽ chạy thẳng vào RUNNING.

- BLOCK-READY: là khi 1 sự kiện xảy ra mà nó có priority thấp hơn RUNNING Task thì nó sẽ vào READY để đợi thoi.

***Process vs Thread:**
- Process tức là 1 chương trình sẽ có vùng nhớ riêng, tức là các process sẽ không liên quan gì đến nhau. (2 CPU)
- Đối với Thread thì nó nằm trong process tức là các thread sẽ có vùng nhớ chung, nó chỉ khác mỗi stack thoi. Và cũm có thể nói Thread chính là các task trong chương trình. (1 CPU)


***Scheduler Classification:**
Này nó là các phân loại kế hoạch lập lịch như dựa vào thời gian, tính định trước hoặc cách hoạt động. Như trong bài ta có Static Scheduling, Dynamic Scheduling và Preemptive Scheduling, Non-premptive Scheduling.

- Static Scheduling, Dynamic Scheduling: Phân loại theo thời điểm quyết định lập lịch với Static sẽ được lập trước khi compile-time hay trước khi chạy, phù hợp trong hard real-time, nơi mà tính toán thời gian cực kỳ quan trọng. Còn trái lại vói Static là Dynamic thì thời điểm quyết định được thực hiện khi hệ thống đang chạy run-time.
(Dynamic là kiểu t không biết task nào sẽ chạy tiếp ấy, kiểu ví dụ như static nó kiểu fix cứng task A -> task B -> task C ròi quay lại task A, còn dynamic sẽ kiểu đang chạy ở task A nhưng nó sẽ không được fix cứng task tiếp theo nó chạy là gì, nó chỉ được biết khi quá trình context switching diễn ra, nó mới biết được task tiếp theo nó chạy là task gì -> Việc dynamic sẽ tốt hơn vì đương nhiên trong thiết kế sẽ có nhiều task ẩn phải có ngắt mới wake up. Việc ta static fix cứng task A -> task B thì khi task ẩn ví dụ là task C được bật lên, thì task C nó cũm đâu được chạy vì scheduler của cta bị fix từ A sang B ròi lại quay về A. Còn nếu là Dynamic thì khi task C xuất hiện là dù ở task A hay ở task B nó cũm sẽ chủ động vào task C trước, ròi mới quay lại scheduler).
- Preemptive Scheduling, Non-Preemptive Scheduling: Như cái tên thì 1 cái ưu tiên ngắt, 1 cái không ngắt. Với Preemptive thì 1 tác vụ có thể bị gián đoạn bởi 1 tác vụ có độ ưu tiên cao hơn, dùng nhiều trong RTOS. Còn Non-preemptive không ngắt hay là 1 tác vụ này thực hiện xong thì tác vụ khác mới thực hiện hoặc là tác vụ đấy phải tự nguyện mới chuyển sang task khác, vậy nên nó không phản ứng nhanh với tác vụ ưu tiên cao, dẫn tới hệ thống không đồng bộ.

- Các group of classification (nhóm phân loại) dựa vào các phân loại mình nói trên ta sẽ có: Dynamic preemptive algorithms, Static Preemptive algorthms, Dynamic Non-preemptive algorithms, Static Non-preemptive algorithms.

***Preemption:**
Thì í muốn nói thì như nào là preemption ? Là khi OS di chuyển thread từ RUN State về Ready State, tức là khi có 1 ưu tiên ngắt cao hơn xảy ra thì cái thread hiện tại sẽ phải về Ready State. Vậy sao từ Run State đến Blocked State không thể hiện là preemption? Thì cái từ Run State đến Blocked State nó xảy ra khi cái thread này thực hiện xong ròi, vậy có khác dì thực hiện tuần tự đâu, kiểu 1 task xong lại đến task khác. Vậy nên mới bảo preemption scheduling sẽ là RUN State về Ready State. Vậy tại sao OS lại làm như vậy, tạo ra preemption algorithm? Thì Preemption is needed to guarantee fairness(cần để đảm bảo sự công bằng giữa các thread như thời gian chạy và công bằng về độ ưu tiên), Preemption needs an interrupts. Preemption helps meet deadlines (giúp đáp ứng giao hạn cho từng thread).

***Scheduler Criteria (Tiêu chí lập lịch / Tiêu chuẩn):**
Đây là những tiêu chí hay tiêu chuẩn để đo mức độ hiệu quả, cũm như phù hợp của 1 scheduler với hệ thống.

- Throughput nói về số task thực hiện được trong 1 đơn vị time hay trong 1 khoảng thời gian thực hiện được bao nhiêu task (thông lượng).

- Turnaround time (thời gian hoàn thành): là thời gian mà 1 task thực hiện, từ lúc bắt đầu đến lúc kết thúc (time hoàn thành 1 task này thường không cố định nếu như xảy ra ngắt). 

- Response Time(thòi gian phản hồi): là thời gian từ lúc nó được gọi ra đến lúc nó chạy. Tức là nó được gọi ra ròi đúng không nhưng vì có 1 task có độ ưu tiên cao hơn nên nó phải chờ đến lượt nó chạy. Thì thời gian nó chờ đó chính là response time.

- CPU Utilization: là tỷ lệ phần trăm thời gian mà CPU bận làm việc, so với tổng thời gian hệ thống chạy. (nên tìm cách để CPU sleep càng nhiều càng tốn -> sẽ đỡ hại CPU hơn). Ví dụ CPU chỉ chạy 10p trong 1 tiếng chẳng hạn, còn lại là ngủ hehe. Tại trên thực tế í 1 task không nên lặp đi lặp lại như vậy tốn CPU, nó sẽ phải check liên tục, vậy nên khi thiết kế 1 task khi nó làm xong việc CPU sẽ ngủ tức là cho task sleep ấy (BLOCKED), và nó sẽ chỉ chờ 1 cái wake up (cái wake up này mình tự thiết kế), thì khi wake up cái task đó mới chạy tiếp.

- Wait Time: là tổng thời gian thread nằm trong hàng đợi chờ được chạy.

***CPU Utilization**

Bài này xem video vì có công thức, t chỉ giải thích mấy cái khó hiểu ở bên dưới thoi, chứ không ghi chi tiết công thức được

Nhưng mà cơ bản nó nói về thời gian tối ưu của 1 task và cách tính toán time CPU chạy và theo đọc chat GPT thì các task không nên chiếm quá 60% CPU.

- Maximum time to execute each task: Tức là như ta biết các task mỗi lần chạy thì cái time để hoàn thành task đó nó sẽ khác nhau. Ví dụ task A lấy dữ liệu từ sensor, thì cái biến số ở đây chính là cái sensor, vì sensor nó xử lý dữ liệu là không hoàn hảo, ví dụ như ta biết cảm biến dht22, thì nó sẽ có dải thời gian để nó xử lý thông tin (20-40us đọc datasheet sẽ thấy dữ liệu này). Nghĩa là con MCU và con sensor không phải lúc nào nó cũm tối ưu time 100% nó sẽ có những sai số, như resistor còn có sai số được mà. Đó thì cơ bản cái maximum này ta phải test thoi :)) chứ khó nói.
- Period task sau bao nhiêu thời gian task chạy lại. (Ví dụ 10s thì đọc lại sensor 1 lần)
- Còn Period of tasks sau bao nhiêu lâu thì các task quay lại 1 chu kỳ mới 

***Scheduling Algorithm optimization:**
Thì cơ bản nó sẽ nói về việc tới ưu những cái gì để scheduler đạt hiệu quả cao nhất.

- Maximum Throughput: tức là trong vòng 1s chạy được càng nhiều task càng tốt (Ví dụ có 3 task, mỗi task đọc 1 sensor, thì trong 1s nếu tối ưu thì 3 task đều đọc được sensor, còn nếu không tối ưu thì trong 1s chỉ đọc được 1 task -> không ổn, mặc dù nhiệm vụ chỉ là đọc sensor. Thông thường các task đọc xong sẽ SLEEP nhớ, chứ ta sẽ không cho nó chạy liên tục, nhưng mà ý dòng này t muốn nói là nên tối ưu các task, để cho ví dụ 3 task cùng chạy thì thời gian để chạy 3 task càng nhanh càng tốt để CPU còn nghỉ ngơi).

- Minimize Turnaround time: Thời gian hoàn thành 1 task phải nhỏ nhất (thì cái này dễ hiểu thoi, time hoàn thành càng nhỏ task chạy càng nhanh mà).
- Minimize Response Time
- Maximum CPU Utilization
- Minimize Scheduling Overhead: Tức là thời gian chuyển task này sang task khác phải nhỏ nhất(tức là càng nhanh càng tốt).


Ngoài ra ta cần biết thông tin application để tối ưu code như:
- Number tasks
- Resourse requirement each task
- Release time for each task
- Deadlines for each task

Và biết thêm các thông tin platform(chính là các bước trung gian như kiểu context switch , IRQ ấy) tức là ta phải biết cái độ trễ lớn nhất của context switching, IRQ (Iterrupt service latency) và độ trễ lớn nhất cả các giao thức (UART, I2C), nhờ đó ta sẽ tính ra được thời gian cần tối ưu


***Overview of popular scheduling algorithm:**
Tức là nhưng cái thuật toán này, là kiểu trên thế giới người ta đã tìm ra nhưng thuật toán này là nhưng phương pháp tối ưu cho RTOS. (còn nhiều thuật toán nữa nhưng mà 5 cái này là tối ưu)

- First Come First Serve (FCFS)
- Round Robin(RR)
- Weighted Round Robin(WRR)
- Rate Monotonic Scheduler (RMS)
- Shortest Job First(SJF)

***First Come First Serve Scheduling**

Đây là thuật toán kiểu cái nào đến trước thì chạy trước, và nó non-preemptive (tức là chạy theo tuần tự ấy không bị ngắt).
Ví dụ Task A vào trước thì phải chạy hết task A mới đến task B, nch là cái nào vào trước thì chạy trước và không có priority, cứ chạy hết cái này ròi đến cái khác :)), kiểu nó như chương trình polling ấy, nghe là thấy cái thuật toán này quá nhiều thứ không ổn :)) ví dụ task 1 nó là while(1) thì sao :)) thì không đến lượt task 2. Còn ưu điểm của thuật toán thì nó dễ dùng thoi kiểu nó cho các task vào queue xong task nào vào trước thì chạy trước, rất dễ thực hiện.

Thì ở trong bài có ví dụ về việc tính time:
Có 4 thread, có các thông số như arrival time, burst time giờ ta sẽ tính như service time, wait time, turnaround time

Thì để xem cách tính ta sẽ xem video, nhưng mà ta sẽ giải thích thêm
- arrival time thì nó là thời gian tới của thread. Ví dụ như thread A chạy ở 0ms, thread B chạy ở 1ms, ... Tức là sau khi thread A chạy được 1ms thì Thread B mới bắt đầu chạy. Chứ arrival time nó không phải là thứ tự chạy đou.
- burst time: là time mà thread đó chạy, ví dụ thread A là 5 tức là 5ms ấy, sau 5ms nó mới chạy xong.

-> sau khi nhìn burst time ta sẽ nhìn được luôn tổng số thời gian chạy của 4 thread (Throughput): 5ms + 3ms + 8ms + 6ms = 22ms, tức là sau 22ms mới chạy xong 4 thread.

- còn service time là thời gian bắt đầu phục vụ thoi, thì với task A nó chạy ngay từ 0ms. Còn task B nó phải chờ task A chạy xong (5ms) -> task B phải đến 5ms. Còn task C phải chờ cả A và B chạy xong, task A chạy 5ms, task B chạy 3ms -> task C 8ms mới bắt đầu chạy.

- wait time thì là time chờ của 1 task ví dụ với task B thì ta có arrival time của nó là 1ms nhưng mà đến 5ms nó mới bắt đầu được chạy, vì phải chờ task A chạy xong -> task B phải chờ 4ms. Tương tự ta sẽ tính time các task còn lại.

- turnaround time; tức là time để hoàn thành 1 task. Ví dụ như task B, nó chạy từ 1ms nhưng mà nó phải đợi task A chạy xong ròi nó mới được thực hiện. -> ta sẽ tính từ nó lúc nó bắt đầu chờ, đến lúc nó chạy xong. Bắt đầu chờ từ 1ms và đến 8ms(5ms của task A và 3ms của nó) mới chạy xong -> thời gian nó hoàn thành là 7ms.

Còn tính wait time trung bình là để so sánh độ hiệu quả giữa các thuật toán, trung bình user sẽ phải đợi bao lâu để hoàn thành 1 task.

Ncl thuật toán này nó không tối ưu vì các task phải đợi quá lâu để đến lượt, xong nó lại theo thứ tự nữa :))

***Round Robin(RR) Scheduler**

Thì đây là thuật toán có lẽ tối ưu hơn mà chúng ta vẫn thường biết đến đối với RTOS

Thì về cơ bản thuật toán này sẽ sử dụng time quanta, hay có thể nói time này mức time định sẵn kiểu cứ 10ms là nó sẽ check chuyển task 1 lần. Thì mới việc này mỗi task chỉ chạy 10ms là đổi task khác, xong cứ như thế đến lúc chạy xong và đây là thuật toán preemptive, tức là task này chạy chưa xong, nhưng mà phải nhường cho task khác vì điều kiện gì đó (ở đây điều kiện chính là time quanta 10ms sẽ đổi sang task khác). Thì lưu ý là với thuật toán round robin là cta chưa có khái niệm về priority, nó chỉ có khái niệm queue thoi. Khi xem video ta sẽ thấy task A khi chạy xong 10ms nó sẽ đẩy vào cuối của queue. Ta sẽ nói qua ví dụ xuống bên dưới.

VD như trong video: Task A nó sẽ bắt đầu với chỉ 1 mình nó vì task B, task gần với nó nhất có arrival time là 40ms. -> Task A lúc này sẽ 1 mình chạy đến 40ms, thì lúc này task B xuất hiện và đưa vào ready queue, còn Task A sau khi chạy xong 40ms nó sẽ bị đẩy ra đằng sau của queue (queue lúc này mới có 2 task A và B). Đó thì giờ có 2 task, thì 2 task sẽ cứ 10ms chạy vào của nhau. Tức là task B chạy được 10ms lại chạy vào task A, task A chạy 10ms sẽ vào task B. Và nếu có 3 task thì nó cũm xen kẽ như vậy, còn việc đến lượt task nào chạy thì phải dựa vào queue. Như t vừa nói task A nếu chạy chưa xong nó sẽ đưa về cuối queue, cứ như vậy với nhiều task chưa xong (xem video để thấy cái nhìn tổng quan). Còn nếu đang chạy và gặp 1 task giờ mới arrival ví dụ như task A đang chạy, gặp task C giờ mới arrival, thì task C sẽ được đưa vào queue trước ròi mới đến task A đưa vào queue.

Ngoài ra ta muốn xem tính toán wait time các kiểu thì có thể xem video thì cơ bản : Average Wait Time trong video là 8.5ms và 0.18 threads/ms còn với FCFS thì nó chỉ 5.75ms và cũm 0.18 thread/ms nhưng kể cả dù vậy nó cum không phải thứ đánh giá tất cả :))

***Quanta**
Oke thì bài này nói về time quanta cụ thể, thì trước tiên ta sẽ giải thích các ý trong video
- Extremely large time quanta, RR = FCFS: í muốn nói nếu time quanta cực lớn thì nó sẽ không khác gì FCFS, tức là ví dụ 22ms chạy 4 thread, giờ t cho time quanta lớn hơn tổng time chạy của các thread ví dụ 50ms :)) thế thì còn tác dụng dì đâu, các thread sẽ chạy lần lượt giống FCFS.
- Extremely small time quanta, RR = True processor sharing: Còn đây là nếu để time quanta cực nhỏ VD: 0.00001 thì nó sẽ làm các thread ấy chạy nhìn như //, tại cứ 0.00001 là nhảy vào thread khác mà, gần như trong cùng 1 thời điểm các thread đều chạy. Nhưng mà lí do bên dưới khiến ta khó làm thế.
- Smaller time quanta = More time spent Context Switching: thì đây ý muốn nói time càng nhỏ tốn càng nhiều time Context Switching, và đó là lí do ta không để time quanta cực thấp được, vì thread chạy được 1 tí, xong lại sang context switching -> CPU tốn nhiều time vào context Switching -> không ổn.
- Time quanta must much greater than Context-Switch time: thì đây time quanta phải lớn hơn time context switching đấy là gần như bắt buộc, vì ta thiết kế RTOS để tối ưu thời gian, CPU cho các task, chứ không phải là dành time cho context switching.

Thì ta sẽ ví dụ Context switch = 10µs, Quanta = 20µs, thì tức là task chạy 20us là lại mất context switching 10us tức là context swtiching chiếm tận 33% tổng thời gian trước khi chuyển task. Và việc context switching nhiều còn làm việc tăng thêm time CPU phải hoạt động. Ví dụ nếu ta tính toán hợp lí thì chạy 3 thread mất 10 lần context switching chẳng hạn -> 10*10us = 100us -> nhỏ và CPU không bị nó chiếm không quá nhiều time. Còn nếu ta để time quanta rất nhỏ thì với 3 thread kể thể phải context switching 100 lần :)) -> gấp 10 lần chỉ để context sw.

Vậy việc chọn time quanta là bao nhiêu là hợp lí?  Thì phải dựa vào chương trình của mình nó có những gì các thread của mình hoạt động bao nhiêu time.
- Thứ nhất time Quanta phải lớn hơn time context switching là chắc chắn, thường thì phải gấp 10 đến 100 lần context switching.
- Thứ 2 là chọn time quanta sao cho tối ưu respone time, tức là tối ưu thời gian yêu cầu của người dùng. Ví dụ người bthg không cảm nhận được độ trễ 100ms, tức là ta có thể cho LCD tạm thời ngủ dưới 100ms, ròi dành cho CPU làm những công việc khác nặng hơn. -> time quanta nên chọn dưới 100ms và tùy ứng dụng thread, ví dụ với thread cần phản hồi nhanh, thì cho time quanta thấp tầm 10ms là sẽ ổn.
VD: 
```C
T1 = 100ms
T2 = 5ms
T3 = 5ms
ở ví dụ này trong thread mình có 2 ứng dụng chạy 5ms là xong thì ta để time quanta là 5ms cũm được, xong ròi nhường CPU cho Thread 1. Chứ nếu cộng trung bình :)) 100+5+5 thì sẽ tầm 36s :)) Thread 2 và 3 sẽ bị delay rất dài.
-> ta nên tùy vào ứng dụng mà đưa ra time quanta hợp lí
```

***Weighted Round Robin (WRR) Scheduler**
Oke nói về cái weighted này thì cơ bản nó cũm kiểu cùng tính chất với round robin thuần, tức nó là preemptive, sử dụng time quanta. Chỉ có cái khác cũm là cái quan trọng nhất là nó sử dụng weighted.

Thì cái weight này là nó để nói về độ quan trọng của 1 task. Task đó mà quan trọng tùy thuộc vào weighted tương ứng, ta sẽ kéo dài time của task đó hoặc làm cho nó xuất hiện nhiều hơn. Thì ví dụ ta sẽ nói bên dưới

Oke đầu tiên ta sẽ có 2 cách chia weight
- Đầu tiên là varing quanta tức là ví dụ weight của thread C bằng 3 đi, quanta của chươnng trình là 10ms cho các thread. Thì với việc thread C bằng 3 weight thì thread C sẽ chạy 30ms khi mà đến lượt nó, còn các thread khác thì vẫn 10ms thoi. (xem video để có cái nhìn trực quan).

- Varing frequence: thì đây như cái tên là thay đổi tần số xuất hiện của thread. Thì như bên trên thay vì thay đổi time quanta lên 30ms, thì ở đây ta sẽ giữ nguyên time quanta là 10ms cho cả thread C, xong ta sẽ để thread C xuất hiện 3 lần trong queue. Thì trong trường hợp nếu ta để 3 cái thread C cạnh nhau :)) thì nó là lại thành varing quanta (T1 - T2 - T3 - T3 - T3 - T4). Còn nếu thiết kế đúng ta nên để thread C xen kẽ giữa các thread (T1 - T3 - T2 - T3 - T4 - T3), đó thì như này thread C xuất hiện đúng 3 lần và không bị varing quanta.

Thì ưu nhược điểm của 2 cách này, với cách đầu tiên là varing quanta thì ưu điểm chắc là dễ thiết kế, còn nhược điểm là nó sẽ gây delay đặc biết với weight lớn ví dụ với weight 100, nếu thế thì nó có khác gì polling hay FCFS đou, tức là các thread sau phải chờ ẻe mới đến lượt. Còn với varing frquence chắc toàn ưu điểm thoi, còn nhược điểm thì chắc phải nghĩ cách thiết kế queue cho hợp lí.


***Internals of the Round Robin Scheduler**
Về cơ bản là nó nói về luồng code của chương trình
Thì ta sử dụng struct tcb để thực hiện ở thread. Thì cái struct tcb này t nói ở bài 9. Thread Control Block(TCB) ròi. Thì video này nó chủ yếu nói về giải thích *stackPt và *nextPt khá nhiều, nhưng mà nó nói khá khó hiểu đặc biệt từ 2p20 trở đi kiểu nó dịch bị sai và nói khó nghe (nhưng cơ bản đoạn này t cảm giác nó đang nói về cơ chế lưu register và register bank vào stack mà thread đó sở hữu) :))
Ncl cơ bản sẽ là 4 task chạy theo cơ kế Round Robin. *nextPt thread 1 thì trỏ tới struct tcb của thread 2, cứ tương tự như thế đến thread 4. Còn cái stackPt là nó lưu địa chỉ stack của nó ròi :)) nma trong video nó cứ bị nói khó nghe :)) hình như nó thay context = stackPt :)) nên là bị loạn, kiểu hết context xong context switching, nhiều từ quá không biết cái nào vào cái nào. Ncl video này tham khảo, tốt nhất nên xem code ở bài tiếp theo để hiểu hơn, chứ khái niệm ở bài này hơi loạn, dù nó nói đúng cơ chế, nma cái stackPt thì không thấy nói (như t nói hình như nó bị gắn bằng từ context).

</details>

<details><summary> <h1> 11. Context Switching </h1></summary>

Cơ bản nói về Context Switching thoi, cái này ta biết khá rõ ròi, thì khi ta kết thúc thread thì register và register bank lưu vào stack frame, và ngược lại khi ta chuẩn bị vào thread ta sẽ lấy các value đó từ stack frame vào register và register bank để chạy.

NCL có thể xem video để cho hiểu mấy cái thuật ngữ.

</details>

<details><summary> <h1>12. Kernel </h1></summary>
oke thì bài này nói tổng quan về kernel làm các nhiệm vụ như 
- Thread Scheduling: thì cái này chính là lập lịch cho thread ấy, quyết định thread nào chạy, chạy bao lâu và khi nào ngắt
- Booting: Tức là kernel sẽ làm nhiệm vụ booting ấy, nó là các code làm việc kiểu reset xong khởi tạo stack pointer, khởi tạo bảng vector table ấy ... làm mọi thứ cho đến khi chạy main() -> việc quá trình booting đó là nằm trong nhiệm vụ của kernel.
- Synchronization(đồng bộ): Tức là nhiều thread dùng chung 1 bộ tài nguyên, ví dụ 2 task cùng dùng chung 1 UART, nếu mà ta không đồng bộ thì 2 task sẽ bị tranh chấp tài nguyên gây lỗi (race condition) thì nhiệm vụ của kernel cũm phải quản lý các tài nguyên chung.
- Inter-thread communication (giao tiếp giữa các thread): tức là kernel sẽ có nhiệm vụ làm cầu nối giữa các task. Vì dụ task 1 cần dữ liệu từ task 2, thì kernel sẽ giúp làm việc đó. Hoặc trường hợp ta hay biết nhất khi IRQ xảy ra thì wake 1 task ấy, cũm là giao tiếp từ kernel tới thread.
</details>

<details><summary><h1>13.RTOS Scheduler Using Systick Exception only </h1></summary>

Thì ta nói qua về BSP trước đã thì về khái niệm BSP là Board Support Packet là tập hợp các mã nguồn và cấu hình giúp 1 RTOS hoặc 1 OS chạy được trên 1 phần cứng cụ thể.
Thì nghe hơi khó hiểu thì giờ ta có câu hỏi thì làm sao để RTOS hay 1 OS chạy được?? Thì RTOS này phải truy cập vào phần cứng đúng không, phải biết core là gì xong trỏ tới thanh ghi để set time, ròi mới Round Robin cho các thread được. Thì đó BSP m cứ coi nó là 1 cái API được đóng gói dành riêng cho OS và RTOS, thì cái BSP này sẽ giúp m truy cập vào phần cứng và set time. Còn đối với RTOS nó chỉ biết là gọi hàm time_systick(10) chẳng hạn, nó không quan tâm phần cứng của nó là gì.
-> Tổng kết thì BSP chính là phần bare-metal tối thiểu để RTOS hay OS chạy được (BSP sẽ cung cấp các hàm init phần cứng như init systick, và cung cấp các API truy cập phần cứng, còn RTOS và OS chỉ việc chạy thoi).

Ví dụ về BSP: Như với stm32 đó là Startup file (startup_stm32xxxx.s), SystemInit(), Clock config, HAL/LL drivers -> thì những cái này nếu t dùng các IDE thì nó viết sẵn hết ròi, mình gần như chỉ cẩn ứng dụng RTOS vào.

**OK giờ sẽ đến phần code dựa vào video**
Oke đầu tiên về cách setup thì ta sẽ xem video 2 sẽ hướng dẫn cách set up với stm32f4


### osKernel.h

```h
#ifndef __OS_KERNEL_H__ 
#define __OS_KERNEL_H__

/*launch os*/
void osKernelLaunch(uint32_t quanta);

/*init os*/
void osKernelInit(void);

/*add thread - thì ở bài này ta sẽ fixed cứng là chạy 3 thread, còn thread dynamic ta sẽ làm ở những bài sau
* @Return uint8_t trả về số lượng mà thread chạy thành công
*/
uint8_t osKernelAddThreads(void (*task0)(void),void (*task1)(void), void (*task2)(void));

#endif
```

### osKernel.c

```c
#include "osKernel.h"

/*thì đây là ta sẽ truy cập vào thanh ghi System Handler Priority Registers của System Control Block. 
Thì trong tài liệu core m3 register có ghi là các exception được phép config priority chỉ ngoại trừ Reset, HardFault, and NMI exception -> ta sẽ truy cập vào thanh ghi SHPR3 ở đó nó sẽ cho phép ta config Systick và PendSV exceptions, thì ở đây ta chỉ config Systick trước (thực ra ứng dụng ở bài này là lấy value ra, chứ không phải ghi vào). Thì Systick có rải bit từ 24 đến 31*/
#define SYSPRI3             (*((volatile uint32_t*)0xE000ED20))

#define BUS_FREQ            16000000
uint32_t MILLIS_PRESCALER;


#define NUMBER_OF_THREADS   3
#define STACKSIZE           100

void osSchedulerLaunch(void);

struct tcb{
    uint32_t *stackPt;
    struct tcb *nextPt;
};

typedef struct tcb tcbType;
/*mảng quản lý địa chỉ các struct của thread*/
tcbType tcbs[NUMBER_OF_THREADS];

/*trỏ tới thread hiện tại*/
tcbType *currentPt;

/*tạo stack cho các thread*/
int32_t TCB_STACK[NUMBER_OF_THREADS][STACKSIZE];

/*Init Stack cho các thread, init top stack ấy
* @Param i: điền thread muốn init, ở đây có 3 thread thì điền 1 trong 3
* @explain: thì -16 ở đây là 16 * 4 = 64 byte nhơ :)), chứ không phải 16 byte clm. Tức là nó trừ đi 32byte tự động là R0,R1,R2,R3,R12,PC,LR,xPSR. Ròi nó trừ tiếp đi 32 byte từ R4-R11 nữa. Tức là khi mà IRQ xảy ra nó sẽ lưu 64 byte nha, nên là -16 là chuẩn òi
* Vậy tại sao mới init mà đã -16 byte và thiết lập thumb mode. Thì khi vào chương trình CPU không biết đây là lần đầu của cta vào thread, nên là nó sẽ restore context từ stack. Giờ m tưởng tượng thì lần đầu vào thì cái tcbType *currentPt; nó chưa biết trỏ tới đâu. Nma theo đúng quy trình khi vào 1 cái thread là phải restore trc (tại CPU nó được cấu hình là làm vậy), và đúng quy trình khi ra khỏi thread nó sẽ lưu các cái context sang stack. Thì như t nói lần đầu mình vào thread, CPU không quan tâm là lần đầu, nó cứ làm đúng quy trình là restore. -> mình phải tạo ra hàm init tự tay viết 1 stack frame giả lập trước để cho CPU nó lấy. Những thông số cần ghi vào Init đó là thumb mode và PC sẽ trỏ tới vị trí đầu tiên của thread, còn những giá trị R0-R11 mình có thể ghi những giá trị bất kỳ :)). Vì trên thực tế mình sẽ không bao giờ return các giá trị rác đó đou. Kiểu m phải tưởng tượng là nếu giờ m chạy trong main ròi thì các thread cứ luân phiên nhau chạy, hoặc có ngắt xảy ra -> nó se không bao h chạy hết ấy :)) còn khi chương trình kết thúc là ta return 0 luôn òi, hoặc khi reset là nó ấy lại từ đầu.
*/
static void osKernelStackInit(int i){
    /*-16 tức là 16*4= 64 byte, mỗi thanh ghi 4 byte*/
    tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE - 16];
    /*set thumb mode*/
    TCB_STACK[i][STACKSIZE-1] = 0x01000000;
}
/*trong đây sẽ gọi hàm osKernelStackInit và set con trỏ PC trỏ tới hàm, ngoài ra phải disable ngắt để quá trình khởi tạo stack không bị lỗi
*/
uint8_t osKernelAddThreads(void (*task0)(void),void (*task1)(void), void (*task2)(void)){
    __disable_irq();
    tcbType[0].nextPt = &tcbType[1];
    tcbType[1].nextPt = &tcbType[2];
    tcbType[2].nextPt = &tcbType[0];
    osKernelStackInit(0);
    TCB_STACK[0][STACKSIZE-2] = &task0;
    osKernelStackInit(1);
    TCB_STACK[1][STACKSIZE-2] = &task1;
    osKernelStackInit(2);
    TCB_STACK[2][STACKSIZE-2] = &task2;
    currentPt = &tcb[0];
    __enable_irq();
    return 1;
}

/*Init Systick time*/
void osKernelInit(void){
    __disable_irq();
    
    /*set 1ms*/
    MILLIS_PRESCALER = BUS_FREQ / 1000;
}

/*cho Systick chạy
* SYSTICK này từ thư viện stm32f4
*/
void osKernelLaunch(uint32_t quanta){
    SYSTICK -> CTRL = 0;
    SYSTICK -> VAL = 0;
    /*set priority for systick*/
    SYSPRI3 = ((SYSPRI3 & 0x00FFFFFF) | 0xE0000000) // priority 7

    /*tức là muốn bao nhiêu ms, MILLIS_PRESCALER = 1ms*/
    SYSTICK -> LOAD = (quanta*MILLIS_PRESCALER) - 1;
    SYSTICK -> CTRL = 0x00000007; // bắt đầu chạy
    osSchedulerLaunch();
}
```

### osKernel.s

```s
/**
* Đầu tiên dòng đầu ta muốn bảo là set đoạn code bên dưới vào vùng này của stm32 f4. Tức là m biết đấy là các code nạp vào vi điều khiển đều phải thông qua linker file để linker file đặt vị trí code đó vào RAM hoặc FLASH và vùng nào. VD như code dưới đây là vùng .text code, READONLY và có Align = 2
* Còn dòng 2 là muốn nói các lệnh này là lệnh thumb mode
* Còn dòng 3 thì đơn giản là extern sang file khác để lấy biến currentPt
*/
            AREA|   .text|,CODE,READONLY,ALIGN=2
            THUMB
            EXTERN  currentPt               // EXTERN là lấy hàm hoặc biến từ file C 
            EXPORT SysTick_Handler
            EXPORT osSchedulerLaunch        // EXPORT để cho các file C khác lấy được hàm
SysTick_Handler                     // hàm ngắt SysTick
    CPSID        I                  // Disable interrupt (mask IRQ).

    /*Lưu context của thread trước khi chuyển task*/
    // Push R4-R11 vào Stack -> lúc này hoàn thành full context R0 - R12
    PUSH         {R4-R11}           
    LDR          R0, = currentPt;  // Load địa chỉ của currentPt vào R0
    LDR          R1, [R0]          // Load giá trị của currentPt vào R1
    // Lưu SP, do stackPt ở vị trí đầu tiên của struct, nên là nó lưu vào biến stackPt luôn, còn nếu muốn cộng lên thì ta viết: STR SP, [R1, #4]
    STR          SP, [R1]

    /*Lấy context và tcb của thread kế tiếp*/
    // lấy next tcb, từ việc có địa chỉ next tcb lấy stackPT của thread kế tiếp
    LDR          R1, [R1,#4]     ;R1 đang giờ lưu currentPt->nextPt
    STR          R1, [R0]        ;R0 = R1 -> currentPt chuyển sang thread kế
    LDR          SP, [R1]        ;Lưu stackPt của thread kế tiếp vào SP.
    POP          {R4-R11}
    CPSIE        I
    BX           LR

osSchedulerLaunch
    /*R0 R1 R2 R3 R12 LR PC xPSR*/
    LDR          R0, = currentPt;
    LDR          R2, [R0]
    LDR          SP, [R2]     // SP = currentPt->stackPt
    POP          {R4-R11}     // POP vào thanh ghi do đây là function bthg -> thủ công
    POP          {R0-R3}      // POP vào thanh ghi do đây là function bthg -> thủ công
    POP          R12
    ADD          SP,SP,#4     // cộng thêm 4 lúc này trỏ tới PC ở trong stackPt
    // cho PC của tcb->stackPt pop vào thanh ghi LR -> lúc này ta LR sẽ về task0
    // sau khi POP nó nhảy tới xPSR ròi -> +4 là nhảy vào địa chỉ đầu của stack tcb[0][100]
    POP          {LR}       
    ADD          SP,SP,#4   // về đúng vị trí tcb[0][100] vị trí đầu tiên của stack
    CPSIE        I
    BX           LR         // LR về task0

    ALIGN
    END
```
Giải thích cơ bản những thí diễn ra trong osSchedulerLaunch
function call bình thường khi ở trong hàm khi trả về LR = PC. Các thanh ghi sẽ không tự động push và pop -> phải dùng thủ công ví dụ như trong osSchedulerLaunch ta phải tự POP R0-R3. Nma chỉ trong ứng dụng này thoi còn nếu với function bthg ta không cần push pop gì hết.
Vậy quá trình function call sẽ là:
- Trước khi vào hàm thì chỉ BL func(), tức là chỉ cần nhảy tới địa chỉ của hàm đó là được
- Còn vào trong hàm, tùy thuộc vào hàm, nhưng mà nó thường lưu LR vào stack. Xong ròi khi nhảy ra khỏi hàm thì BX LR hoặc BX PC là nhảy ra khỏi hàm

Thêm kiến thức nữa là trong file.c không thể include file.s -> cần phải sử dụng EXPORT

### main.c

```C
#include "osKernel.h"

#define QUANTA      10

uint32_t count0, count1, count2 = 0;

static void Task0(void){
    while(1){
        count0++;
    }
}
static void Task1(void){
    while(1){
        count1++;
    }
}
static void Task2(void){
    while(1){
        count2++;
    }
}

void main(void){
    osKernelInit();
    osKernelAddThreads(&Task0, &Task1, &Task2);
    osKernelLaunch(QUANTA);
}


```
</details>


<details><summary><h1>14. The CooperativeNon-Preemptive Scheduler</h1></summary>

The Cooperative Non-Preemptive Scheduler nghĩa là bộ lập lịch hợp tác (Cooperative nghĩa là hợp tác)
Này cơ bản là scheduler là kiểu task A chạy hết mới đến Task B


Oke vậy code này tại sao lại là Cooperative Non-Preemptive Scheduler. Thì ở trong bài trước ấy Task 0 sẽ phải chảy đủ 10ms mới context switching sang task khác nghĩa là count0++; sẽ diễn ra trong 10ms. Thì ở bài này với tiêu chí là Cooperative Non-Preemptive Scheduler, tức là task sẽ chủ động tạo ra ngắt. Thì như note ở bên dưới  INCTRL = 0x04000000; sẽ trigger Systick, thì ta có thể tra nó là thanh ghi ICSR bit 26 là PENDSTSET, tức là pending Systick set. Oke vậy sau khi ghi vào thanh ghi là cho Systick Pending thì ta gần như sẽ vào Systick ngay lập tức, không phải chờ đủ 10ms, tức là task đang chủ động gọi Systick, thay vì chờ 10ms -> khi ta xem video ta sẽ thấy các task tự gọi osThreadYield() sẽ tăng chậm hơn, vì mỗi lần count++ 1 lần là nó sẽ nhảy task khác, còn task nào không có osThreadYield() thì nó sẽ chạy đủ 10ms và nó sẽ count++ rất nhiều.
### osKernel.c

```C

#define INCTRL     (*(volatile uint32_t*)0xE000ED04)

void osThreadYield(void){
    INCTRL = 0x04000000; //trigger Systick
}

```

### main.c

```C

static void Task0(void){
    while(1){
        count0++;
        osThreadYield();
    }
}
static void Task1(void){
    while(1){
        count1++;
        osThreadYield();
    }
}

```

</details>



<details><summary><h1>  15.OS Porting Consideration  </h1></summary>

OS Porting Consideration nghĩa là Những yếu tố cần cân nhắc khi chuyển (port) một hệ điều hành sang phần cứng hoặc kiến trúc khác. Thì tức là ví dụ mình bê y nguyên code sang phần cứng khác thì mình phải chỉnh đó. Và cơ bản nó sẽ có BSP, tức là BSP nó đã giúp mình làm việc với phần cứng và mình chỉ vác phần mềm sang và thay đổi theo BSP, BSP giống kiểu quy tắc chung, để cho mình đỡ phải chỉnh lại từ đầu.


***Board Support Package**
Thì ở bài nó giới thiệu qua về BSP, thì cơ bản nó giống như HAL và device driver. Thì các API của BSP sẽ được cung cấp bởi các file.h và được thực thi bởi các file.c hoặc .s được giấu đi. Thì người dùng chỉ được cung cấp dùng những hàm này và nó chỉ dùng được những hàm đó thoi, các hàm static khác sẽ bị giấu

***Path Expression**
Thì như ta đã nói về OS Porting consideration, thì khi ta chuyển sang phần cứng khác, không chỉ nhìn vào BSP và còn hiểu cả Path Expression. Thì Path Expression là 1 kiểu quy trình để chạy 1 module thành công ấy. Ví dụ như trong video nó nói về ADC, thì ADC phải Init() trước xong đến ADC_Getconversion() xong đến ADC_Deinit(). Tức là nếu mà ta gọi hàm ADC_Getconversion() hoặc ADC_Deinit() trước ADC_Init() nó có thể gây lỗi chương trình. Vậy nên ta cũm cần biết sequence nhất định của 1 module.

</details>

<details><summary><h1>16. Board Support Package (STM32F4) </h1></summary>

Phần này ta sẽ tạo ra 1 BSP cho RTOS

Đầu tiên ta sẽ tạo ra file.txt để ghi các pin cần sử dụng, vì ta đang tạo ra BSP mà, thì sự khác biết giữa các board chủ yếu là khác chân pin mà ra.
Thì ở bài này ta sử dụng 2 board đó là STM32F4-DISCO và STM32F4-NUCLEO 

Code bài này thuần xem :)) vì nó dùng HAL khá nhiều, hình như bài này là hướng dẫn cách chạy RTOS bằng việc sử dụng HAL

### Abstract.txt
```txt

**** STM32F4-DISCO Pin Assginment ****

/*PROBE nghĩa là PC0 - PC3 được cấu hình làm GPIO output, và ngoài ra Probe còn có nghĩa là đầu dò - tức là mình sẽ cho nó để dò tín hiệu osci */
PROBE
Probe 0:    PC0
Probe 1:    PC1
Probe 2:    PC2
Probe 3:    PC3

ADC    :    PA4

GLCD_ST7735

SCK         :   PB13
MOSI (SDA)  :   PB15
RS          :   PB14
DC          :   PB12
CS          :   PB10

**** STM32F4-NUCLEO Pin Assginment ****
PROBE
Probe 0:    PA8
Probe 1:    PA9
Probe 2:    PA10
Probe 3:    PA11

ADC    :    PA4

GLCD_ST7735

SCK         :   PB10
MOSI (SDA)  :   PC3
RS          :   PB5
DC          :   PB4
CS          :   PB3


```
</details>

<details><summary><h1>18. The Periodic Scheduler </h1></summary>

Đây là bộ lập lịch các task chạy theo chu kỳ cố định
Ví dụ Task A chạy mỗi 10ms, Task B chạy mỗi 20ms và Task C chạy mỗi 50ms và nó lặp đi lặp lại như vậy -> sẽ là Periodic Scheduler. Và cái periodic này sẽ không phụ thuộc vào Round Robin, tức là cái round nó là task A - B - C. Nhưng mà periodic này sẽ không quan tâm đến việc đó, nó chỉ quan tâm tới time là cứ 10ms hoặc 20ms nó sẽ chạy thread đó.
Yield: nghĩa là tự nguyện (nhường CPU cho các task khác).

Thì như trong video nó bảo có 2 method: (xem video để thấy rõ 2 method)
- Method thứ 1 như trong video là nó sử dụng IRQ, tức là trong video nó sử dụng Timer 3 cứ 100ms là vào IRQ của timer 3 và chạy 1 cái probe 3(hay thread 3)(probe ở đây là chân GPIO nhớ). Tức là nó sẽ sử dụng IRQ để scheduler, và thread sẽ nằm trong IRQ. Thì cách task 0,1,2 nó cứ chạy theo round robin, còn task 3 nằm trong timer 3, sẽ 100ms xuất hiện 1 lần và độ ưu tiên của nó sẽ cao hơn các task kia -> để trong IRQ là cũm hợp lí, vì nó chắc chắn sẽ chạy. (nma ta nên sử dụng phương pháp wake up thay vì gọi thẳng hàm trong IRQ).
- Method thứ 2 là như trong video thì ta không sử dụng IRQ, mà ta sẽ thay cái file.s, thay vì vào đó context switching, thì ở trong đó ta gọi tới 1 cái hàm. Và ở cái hàm đó nó sẽ giúp ta đếm đến 100ms và sử dụng if, nếu = 100ms thì chạy task 3, còn nếu không thì thực hiện context switching với 3 task còn lại task 0-1-2 (ở bài này ta đặt quanta là 1ms ấy, tức là 1ms đổi 3 task liên tục)

Giải thích thêm tại sao là 50% duty. Thì nhiệm vụ của task 3 là đảo chân thì cứ 100ms là nó lại đảo 1 lần. Vậy nên 1 chu kỳ của nó là 200ms, ví dụ nó đang logic là 0 thì 100ms là nó lên 1, xong ròi đếm tiếp 100ms lại về 0.

Tiếp theo ở bài 3 mà ta dùng 2 task 3 và 4 ấy, thì LCM of 100 and 250 = 500 là gì ? Thì nó chính là bội số chung nhỏ nhất của 2 số, tức là 500 đều chia hết cho cả 2 số 100 và 250. Vậy chọn cái bội số chung này để làm gì. Thì nó sẽ nhằm mục đích là khi đến 500 ta sẽ set lại periodic về 0, vì 500 chính là điểm chung của 2 giá trị.

</details>

<details><summary><h1>19. Semaphores </h1></summary>

***1. Introduce to semaphore**

Thì cơ bản semaphore có 2 nhiệm vụ chính:
- Thứ nhất nó sẽ đồng bộ các task: tức là semaphore có cơ chế là thay đổi state của task, nó có thể khiến task từ state READY or RUN sang BLOCKED (WAIT) và ngược lại từ BLOCKED sang READY. Đấy còn việc priority của các task như nào hoặc thứ tự task chạy như nào, thì là do scheduler quản lý, còn semaphore nó chỉ quan tâm tới việc chuyển state như kia thoi.
- Thứ hai nó sẽ quản lý tài nguyên chung, ví dụ 3 task dùng chung 1 UART, thì nó sẽ làm nhiệm vụ là trong 1 thòi điểm chỉ có 1 task được dùng cái UART đó thoi.

Nói thêm về cái semaphore count, thì mấy cái t giải thích ở trên là kiểu semaphore binary. thì nói về semaphore binary trước như nào ở trên thì task nó sẽ có 2 trạng thái 0 và 1 tương đương sema sẽ là sema = 0 và sema = 1. Thì giả sử IRQ nó sẽ kích hoạt task A đk, giả xử IRQ nó sẽ kích hoạt khi có 1 ngắt uart . Đó thì lần đầu tiên ngắt IRQ xảy ra thì task A sẽ từ BLOCK sang READY hoặc RUNNING, và lúc này task A sẽ thực hiện code trong task. Nma câu hỏi đặt ra là nếu trong khi task A đang thực hiện code trong task, thì IRQ lại nhận được tiếp dữ liệu Uart thì sao ?? lúc này task A chắc chắn bị miss dữ liệu, vì nếu ta không có cơ chế lưu dữ liệu lại trong IRQ, thì task A nó sẽ không bao h nhận được dữ liệu đó. Kể cả có cơ chế lưu dữ liệu, nó cũm không biết khi nào nên BLOCKED, vì như ta bảo semaphore binary nó sẽ chỉ 0 và 1, nó sẽ chỉ biết là task đang RUNNING hay BLOCK, chứ không biết là buffer hay số lượng nhiệm vụ nó phải thực hiện. Vậy nên ta cần semaphore count, lúc này semaphore count sẽ nói số lượng nhiệm vụ mà task đó phải làm. Ví dụ khi mà IRQ nó nhận được tiếp uart trong lúc task A đang chạy, thì lúc này IRQ sẽ ++ biến sema_count chẳng hạn, dữ liệu uart sẽ lưu vào 1 buffer. Và nếu mà sema_count lớn hơn 0, thì lúc này task A sẽ tiếp tục chạy task (nma nên nhớ phải trừ sema_count trước khi chạy tiếp), thì nó sẽ check buffer dữ liệu cần xử lý, thông thường dữ liệu trong buffer sẽ  = sema_count.

Nói thêm về sema_count nó sẽ được cộng khi ta gọi give(), tức là trong IRQ sẽ là người tăng sema_count, thì thoát khỏi IRQ thì task mới được thực hiện. Còn sema_count sẽ giảm khi bắt đầu vào task đó, tức là việc giảm sẽ được thực hiện ở đầu task (ngoài ra việc giảm này còn được gọi là take() )

Đây thì trong video nó cũm có nói Implemented as a counter with 3 functions: Init(), SignalWait() and SingleSet(). Tức là í nó muốn nói là semaphore sẽ được thực hiện như 1 counter và sử dụng 3 function kia.
Thì nói thêm về đoạn này thì trong bài muốn semaphore là 1 biến đếm, đầu tiên hàm init() khởi tạo giá trị ban đầu (số token max hoặc token khi bắt đầu là bao nhiêu (token nó giống như biến đếm hay sema = 3 kiẻu như vậy)), SignalWait() lấy token, SignalSet() thêm token. Ta có thể lấy ví dụ như bên trên t ví dụ ấy, thay vì sema_count++ thì ta sẽ gọi SignalSet() để thêm token cho semaphore, và việc SignalSet() này chắc chắn sẽ nằm trong IRQ, còn task nó sẽ gọi wait() để lấy cái token đó để chạy, nếu mà còn token thì task đó sẽ chạy, còn nếu token = 0 thì task đó không chạy được hay tiếp tục bị BLOCKED còn nếu còn token thì task sẽ READY hoặc RUN.

Vậy thông qua đây ta thấy gì, thì với việc quản lý bởi token, thì 1 thread hoặc 1 IRQ có thể kích hoạt 1 task đang BLOCKED thành READY hoặc RUN bằng việc gọi SignalSet

***2.Spinlock Semaphore**
Bài này nói về cơ bản là quy trình của các function, thì trong video có nói SingalWait() sẽ như là 1 cái spinlock, kiểu như là nó sẽ check đi check lại cho đến khi mở khóa ấy, thì mở khóa ở đây chính là chạy vào được 1 thread hay semaphore > 0, còn nếu semaphore <= 0 thì thoi.
và cơ bản nó sẽ giới thiệu cái luồng của các function semaphore như nào, ncl nên xem video để biết thêm. Và ngoài ra cuối video nó còn bảo tới việc cho cái hàm SignalWait() vào 1 thread, thì mỗi quanta(10ms chẳng hạn), nó sẽ vào check SinalWait() và khả năng nó lại check liên tục 10ms :)) đó chính là spin, xoay tròn lặp đi lặp lại.. ncl kiểu như vậy :))

***3.Cooperative Semaphore**
thì cái cooperative này nó giống như ở cái bài 14 ấy, thay vì ta để nó spin 10ms check if semaphore <=0 liên tục, thì giờ nếu if semaphore < 0 cái ta sẽ gọi osThreadYield() (cái hàm này ở bài 14 nói khá rõ) để thực hiện quá trình context switching luôn.

***4.Coding Spin-Lock Semaphore Implementation**

bài này chắc thuần xem thoi, có gì khó hiểu t sẽ giải thích thêm

Thông qua video phần 7 Part 4 ấy thì ta có thể thấy cái Spin-lock này nó như mutex vậy :))
Thì xem video có thể thấy cả 2 task đang dùng chung 1 cái tài nguyên LCD, nếu ta không dùng semaphore thì ví dụ task A nó đang dùng giở thì phải chuyển sang task B và lúc này task B cũm sẽ truy cập vào LCD -> nó sẽ bị xung đột. Vậy nên ta phải dùng 2 semphore, theo như video thì task2 nó sẽ được thực thi trước, nó sẽ SignalWait() semaphore1 vì semaphore1 = 1; (chạy xong hàm Signalwait() semaphore1 sẽ = 0) và khi chạy xong hết lệnh LCD thì task2 sẽ signalSet() semphore2 lúc này semaphore2 sẽ từ 0 lên 1 -> task1 dùng semphore2 nên lúc này có thể chạy, còn trước đó với việc semphore2 = 0, nên dù có nhảy vào task1 thì lệnh LCD cũm không được chạy.

</details>


<details><summary><h1> 20.Inter-Thread Communication And Synchronization </h1></summary>

Này chắc giới thiệu cách giao tiếp và đồng bộ giữa các Thread (nên nhớ là giữa giữa các Thread nhớ)

***1.Rendezvous**
Thì đây nó là 1 cơ chế đồng bộ mà 2 hoặc nhiều thread phải đợi nhau tại 1 thời điểm xác định ròi mới tiếp tục chạy.
Tức là lấy ví dụ dễ hiểu thì Thread A chạy trước nó tới điểm hẹn trước và nó sẽ đứng ở đó đợi Thread B, khi Thread B tới thì ca 2 cùng chạy.

Thuật toán để thực hiện là ta sẽ sử dụng 2 semaphore và cả 2 đều = 0.
Thì khi vào Thread A trước lúc này Thread B chưa được chạy hoặc chạy sau, thì lúc này ta phải cho thread A dừng lại ở điểm nào đó để chờ task B. Thì trong thread A ta sẽ viết signalSet(&semaphore2) lúc này semphore2 set lên 1 (còn việc tại sao set lên ta sẽ nói ở phía sau), ròi tiếp đến ta sẽ gọi signalWait(&semaphore1) thì đây chính là điểm hẹn của thread A, thì vì semaphore1 = 0 nên là đến lúc này thread A sẽ bị kẹt ở đây không thể chạy tiếp. 
Tiếp đến Thread B thì nó sẽ chạy sau thread A, lúc này thread A đang chờ nó ròi. Thread B cũm sẽ thực hiện các hàm tương tự đó là signalSet(&semaphore1) thì việc set này íi muốn nói là thread B đã đến điểm hẹn, và bảo thread A sẽ tiếp tục chạy, vì lúc này semaphore1 = 0 sẽ lên 1, và vì lúc nãy ở thread A ta cũm signalSet(&semaphore2) nên semaphore2 = 1 sẵn ròi, nên là khi threadB gọi signalWait(&semaphore2) là nó sẽ được chạy luôn, không bị phải chờ như ở Thread A.
Và như vậy sau khi chờ xong lúc này 2 semaphore lại = 0, và cùng nhau chạy. Ncl cơ chế này nó bắt ta phải điều chỉnh linh hoạt semaphore 0 sang 1 và 1 về 0.
```C
//Task 1
void task1(void){
    //do st here task A
    while(1){
        // set trước không tí thread A bị Block là khỏi set semaphore2 từ 0 lên 1
        signalSet(&semaphore2);
        signalWait(&semaphore1); // bị block vì semaphore1 = 0; -> wait task2

        // do st
    }

}
//Task 2
void task2(void){
    //do st here task B
    while(1){
        // set semaphore1 tăng -> thông báo cho semaphore1 hay task1 là đã tới điểm hẹn semaphore1 từ 0 lên 1
        signalSet(&semaphore1);
        signalWait(&semaphore2); //nhờ task1 set semaphore2 ròi nên được tiếp tục chạy semaphore2 từ 1 về 0

        // do st
    }

}

Trong trường hợp thực tế nếu task2 chạy trước thì vai trò của nó sẽ đổi với task1, ncl mình cứ linh hoạt thoi, task nào chạy trước thì nhiệm vụ của nó như task1, còn chạy sau thì như task2

```

</details>

<details><summary><h1> 21. Overview of CMSIS-RTOS RTX</h1></summary>

Cơ bản đây là 1 RTOS được phát triển bởi KeilC, nó kiểu giống như FreeRTOS ấy, tích hợp các API dùng cho RTOS. Nên ncl bài này chỉ nên xem thoi, tại vì nếu dùng API tích hợp như này, ngta hay dùng FreeRTOS hơn.

- Small footprint: là kiểu Kernel chiếm ít Flash, tại kiểu nó là thư viện có sẵn mà, nên là vì nó có sẵn thì ta phải xem nó có tốn bộ nhớ hay không, nếu tốn quá thì chẳng dùng để làm gì. Nma RTX và FreeRTOS dều được quảng cáo là tốn ít.

Oke với bài này thì bao h mình dùng đến những API này thì mình sẽ xem.

</details>

<details><summary><h1> 22. Overview of FreeRTOS</h1></summary>

Có lẽ bài này đáng xem hơn bài trên :)) vì nó là FreeRTOS thông dụng hơn khá nhiều, chứ RTX bao h có cty cần thì xem sau
Ncl các khái niệm về FreeRTOS lên chat gpt mà hỏi nhớ.

Tiếp tục là bao h làm thì nghiên cứu kĩ sau và nó cũm khá giống FreeRTOS trong esp32, thì mình có thể làm ở đấy nghiên cứu kĩ hơn

</details>


<details><summary><h1> 23.  Overview of Micrium uCOS-III</h1></summary>

Đây cơ bản là 1 RTOS thương mại, tức là nó dùng cho những ứng dụng đòi hỏi tính chuyên nghiệp cao hơn FreeRTOS. Thì FreeRTOS thường hay dùng cho các ứng dụng startup, Iot, còn cái micrium sẽ dùng cho công nghiệp và y tế, nên đáng để quan tâm.

Ncl bao h làm quả so sánh 2 cái FreeRTOS và cái Micrium này này, nếu job mà mình xin nó yêu cầu biết 2 frame RTOS này. Nma ncl cái Micrium kia khá mạnh, nên sẽ tìm hiểu qua về cái này, và cũm chỉ cần xem video thoi, nó giới thiệu khá nhiều cái.

Cái Task Create() của nó khá nhiều cái :)) tốt nhất là bao h dùng tới thì xem lại video ròi lên chat gpt hỏi các chức năng, có thể trong project tổng của mình sẽ dùng cái này hoặc FreeRTOS hoặc tự tạo, ncl hên xui


</details>

<details><summary><h1> 24. BONUS 1  ARM Cortex-M Assembly Primer</h1></summary>

***1. Assembly Language Syntax**
Thì đây là phần học bonus liên quan đến Assembly, thì đầu tiên ta sẽ nói qua về Syntax cách viết 1 dòng của Assembly trước gồm: 
Label:   Opcode   Operand(s)   ; Comment
thì các lệnh này sẽ được cách nhau bởi space hoặc tabs và Label với comment là các optianl(tùy chọn) có cũm được mà không có cũm được.
Giờ ta sẽ ví dụ và giải thích
```s

start:
    MOV R0, #0

loop:
    ADD R0, R0, #1
    CMP R0, #10
    BNE loop

end:
    NOP

```
- Oke với ví dụ ở trên thì start, loop, end kia chính là label, thì label nó là đại diện cho 1 địa chỉ trong bộ nhớ chương trình, tức là khi nó đặt ở đâu, chính là địa chỉ nó đại diện. Ví dụ với label start kia ví dụ nó nằm ở địa chỉ 0x01 thì mỗi lần ta gọi start là chương trình sẽ nhảy tới địa chỉ 0x01, tương tự với label loop
- Còn Opcode chính là ADD, MOV, CMP ... là những lệnh mà CPU thực hiện
- Operand(s): là nguồn và điểm đích của dữ liệu mà opcode xử lý. Ví dụ ADD R0, R1 thì R1 chính là nguồn dữ liệu và R0 chính là đích đến của dữ liệu, opcode ADD sẽ hướng dẫn ta nên làm gì với 2 dữ liệu đấy
- comment thì đơn giản là comment thoi :)) không ảnh hưởng đến chương trình.

Oke ngoài ra ở cuối video có nhắc tới Directives, thì đây là những lệnh cho Assembler, chứ không phải lệnh cho CPU, tức là CPU sẽ không lấy những dòng Directives để chạy mà Assembler sẽ là người hiểu những cái lệnh  Directives đó. Và những cái Directives này nó sẽ bảo Assembly có code này sẽ nạp vào ở đâu và mode là gì VD như mode Thumb, tức là Directives này nó sẽ điều khiển luồng code ấy, chứ nó không có nạp vào MCU. Lấy ví dụ đơn giản thì giờ nước chính là data hay instruction set, còn ống nước chính là Directives. Ống nước sẽ là người hướng dẫn dòng nước đi đâu, chỗ nào nhanh chỗ nào chậm. Đó thì trong file Assembly lúc này cũm thế, sẽ có data hay instruction set chính là nước, còn Directive chính là ống nước, còn MCU (CPU) chính là người nhận nước, MCU sẽ không quan tâm đến ống nước như nào, nó chỉ nhận nước như nó mong đợi là được. Đương nhiên cái file Assembly kia phải xử lý ống nước để ra kết quả như MCU mong đợi. Giống như ở nhà máy ta không thể lắp 1 cái vòi bé tí cho cả công ty dùng được, vậy nên Directive rất quan trọng trong file Assembly. 
- Thì như trong video có bảo những lệnh này là để hỗ trợ và điều khiển quá trình assembly, nên nhớ là assembly nhớ và chúng được gọi là pseudo-ops. và nó không phải là instruction set code cho CPU, nó đơn giản chỉ là những cái lệnh để ta điều khiển Assembler. Ở bài tiếp ta sẽ ví dụ để cho dễ hiểu hơn

***2.Assembly Directive**
Thì Directive chính xác là làm những gì thì nó dùng để: Tổ chức bộ nhớ (code/data), Tạo biến, Export/import symbol, Căn chỉnh stack, Định nghĩa hằng số, Thiết lập instruction set (Thumb). CPU sẽ không thực thi chúng, mà này là ta nhắc Assembler là làm theo những cái này.

Ví dụ trong startup file hay có ở đầu dòng, hoặc ở cái bài code round-robin ta cũng có code kiểu như này ở đầu dòng, thì đây chính là những Assembly Directive 
```s
PRESERVE8
THUMB
AREA RESET, CODE, READONLY
```

- Thì ví dụ nếu ta ghi THUMB thì chuyện gì xảy ra, thì nếu ta ghi THUMB tức là đã báo cho assembler rằng từ đây assemble theo Thumb instruction set và bảo với CPU đây là lệnh Thumb Instruction, code từ giờ chỉ chạy half-word trong CPU thoi. CPU sẽ chỉ thấy được instruction đã được encoder sẵn là half-word, nó sẽ không thấy chữ THUMB
- Hoặc AREA RESET, CODE, READONLY nó muốn nói với Assembler + Linker là code này nằm ở Flash và chỉ để đọc không thể thay đổi. Và linker script sẽ là người map những cái code đó vào MCU, và đương nhiên MCU sẽ không biết những cái directive này tồn tại, nó chỉ biết là à có dữ liệu đang vào thoi.


Oke đến đây là hiểu Directive là gì ròi, thì giờ trong video sẽ giới thiệu các lệnh Directive cơ bản:
- Thumb: muốn nói code này là Thumb instruction
- CODE: gửi dữ liệu vào ROM
- DATA: gửi dữ liệu vào RAM
- AREA: tạo 1 vùng mới (section) ví dụ như cái AREA RESET, CODE, READONLY, í nó muốn nói là vùng này tên là RESET, instruction đặt ở vùng nhớ CODE (Flash) và nó chỉ dùng để đọc
- PRESERVE8: Đảm bảo stack luôn align 8-byte (tức là dựa vào chỉ thị này assembly sẽ config stack của MCU là align 8 byte luôn, đây là stack pointer nhớ, thì SP phải luôn đảm bảo chia hết cho 8 khi mà function/exception entry, còn nếu push những data bthg vào SP thì sẽ không nhất thiết phải chia hết cho 8, nhưng mà khi cta nhảy ra khỏi hàm hoặc nhảy vào hàm ngắt các kiểu thì theo cấu trúc phải chia hết cho 8 mới đúng, còn với PC sẽ khác, PC có thể align là 2 hoặc 4 byte, không cần phải align 8)
- IMPORT: Lấy từ file khác
- EXPORT: Chia sẽ dữ liệu cho file khác
...
Ngoài ra còn nhiều lệnh nữa thì bao h dùng tới update tại đây

ở bài 3 xem qua cũm được

**4.Project 1 Opcodes**
Oke bài này giới thiệu 7 opcode phổ biến nói chung là những cái lệnh opcode này nó bị gắn chặt với các ý nghĩa là rất lớn ví dụ nhìn MOV và LDR có thể giống nhau, nhưng bản chất sẽ khác nhau, 1 cái register sang register , còn 1 cái memory vào register, đó là những chức năng không thể dùng sai mục đích nên ta cần chú ý như vậy.

- Đầu tiên là MOV: nó sẽ sao chép dữ liệu từ source → destination, nó không tính toán dữ liệu, nó chỉ copy dữ liệu

    VD: MOV R0, R1          - Nghĩa là copy dữ liệu từ R1 sang R0

    Các dạng phổ biến của MOV đó là Register sang register tức là từ R1 sang R0 như ở ví dụ trên hoặc gán trực tiếp giá trị vào

    VD: MOV R0, #100

    Lưu ý là vói register sang register không thể MOV với các high register đó là các thanh ghi từ R8-R12. Ví dụ như MOV R8, #3 là sai. Ngoài ra ta cũm không MOV được giá trị lớn hơn 8 bit. Ví dụ MOV R0, #256 (thì 8bit là từ 0-255) -> #256 là bị lỗi luôn.

- LDR: thì nó dùng để load các dữ liệu từ memory vào register, còn với MOV là chỉ load từ register vào register thoi. Điểm khác lớn nữa là LDR cho phép load 32 bit vào register.

    VD: LDR R0, =0xFFFF         đây chỉ đơn giản là load số vào R0, chứ không bắt buộc phải load register của memory, nma mà nếu ta cần truy cập tới memory thì phải dùng lệnh bên dưới, thì nó ví dụ sau lệnh LDR R0, =0xFFFF là LDR R1,[R0] : tức là trỏ tới thanh ghi mà R0 trỏ tới, nma 0xFFFF không trỏ tới gì cả -> bị lỗi. Còn nếu LDR R0, =0xFFFF đứng 1 mình thì nó đơn giản là load số 0xFFFF vào R0

    VD tiếp: LDR R1, [R2]       thì ở đây là Load giá trị của R2 vào R1, tức là R2 đang trỏ tới 1 địa chỉ và khi ta đóng ngoặc kiểu kia, tức là ta đang trỏ tới cái địa chỉ đó để lấy giá trị bên trong nó, và R1 sẽ lưu giá trị đó.

- STR: Stores a register value into memory
VD: STR R0, [R1]        tức là lưu trữ value của R0 vào R1, nó sẽ tương đương *R1 = R0, tức là R0 sẽ lưu 1 hằng số nào đó ví dụ 100 đi, còn R1 sẽ lưu 1 cái địa chỉ và lúc này ta sẽ trỏ tới địa chỉ đó, lấy giá trị 100 của R0 vào giá trị của R1 nơi mà R1 trỏ tới.

- B: Branch to a location 
VD: B   loop        - tức là nhảy tới loop, tức là cái loop này là đại diện cho 1 địa chỉ, thì ta dùng B này tức là ta muốn nhảy tới 1 địa chỉ, và lúc này con trỏ PC sẽ coi loop là next instruction.

- BL: Branch to subroutine
VD: BL  Turn_on     - call Turn_On và Turn_On là 1 subroutine. Thì subroutine nó là cái gì? Nó là 1 đoạn code thực hiện 1 nhiệm vụ đặc biệt, có thể được gọi và quay lại, trong C ta hay gọi là function hay hàm. Còn trong Assembly ta gọi là subroutine. Thì lúc này ghi ta BL là nó như kiểu gọi tới hàm ấy, thì lúc này nó sẽ nhảy tới hàm và lưu địa chỉ quay lại vào thanh ghi LR. Còn với B loop kia nó đơn giản là nhảy tới 1 địa chỉ bthg thoi, nên không lưu LR làm gì.

- BX    LR: Return from subroutine call
Ví dụ:
```s
BLUE_ON
    MOV     R2, #0x01
    BX      LR
```
Thì ở đây tức là trả về từ Subroutine hay hàm BLUE_ON 

- NOP: No operation, do nothing
VD:
```s

STR   R0, [R1]
NOP
NOP

```
Tức nó có nghĩa là lưu R0 vào R1, xong không làm gì trong 2 chu kỳ -> 1 NOP = 1 chu kỳ. Tức là cơ bản nó không để làm gì nhiều đâu, kiểu mình muốn tạo delay 2 cycle của CPU thì mình dùng thoi.

- ORR R0,R0,#0x20       : ở đây là lệnh OR, thì ta sẽ OR R0 với 0x20
- AND R0,R0,#0x20       : ở đây là lệnh AND, thì ta sẽ AND R0 với 0x20
- EOR R0,R0,#0x20       : ở đây là lệnh XOR, thì ta sẽ XOR R0 với 0x20 (chú ý XOR nó chỉ true khi 2 giá trị khác nhau, còn nếu giống nhau sẽ là 0)

***Coding Assembly**
Thì ở bài 5 sẽ hướng dẫn mình set up KeilC nhớ
Bài 6 sẽ nói về luồng code, thì cơ bản sẽ là bật tắt LED
Bài 7 bắt đầu nói về những thứ phải thực hiện để chạy chương trình, thì đơnn giản là nó hướng dẫn phải setup những cái gì thoi
Thì theo những gì ta biết đầu tiên phải kích hoạt clock cho port thì ở bài này nó nằm ở RCGCGPIO register, và còn phải chờ cho status bit ở thanh ghi PRGPIO bật lên (khả năng cái thanh ghi này nó sẽ thông báo là Port D đã được có clock thì ở đây chỉ có port D mới phải làm thế đối với con vi điều khiển này, ncl cái này khá mới so với con stm32f103 mình hay dùng). Thì sau khi bật clock cho Port D thì ta sẽ Unlock pin PD7 nhưng mà bài này ta dùng Port F. Sau khi unlock thì giờ ta chỉ cần set cho pin là input hay output ở thanh ghi DIR register. Tiếp đến ta sẽ Enable Pin, tức là bằng đầu cho pin hoạt động ở DEN register. Và sau đó cuối cùng ta sẽ điều khiển logic ở thanh ghi GPIODATA register (ON/OFF GPIO ấy)

Mà bài này là ta dùng Port F nhớ :)) và thông qua video ta thấy thanh ghi RCGCGPIO register sẽ set port F ở bit 5, còn ở DIR register thì set lên 1 là output. Còn DEN register chỉ đơn giản là register enable lên thoi, tức là nếu mình cấp clock, set chân các kiểu cho nó mà không bật nó lên thì cũm chả hoạt động được.

```s

SYSCTL_RCGCGPIO_R   EQU     0x400FE608          ; EQU ở đây giống như define SYSCTL_RCGCGPIO_R  0x400FE608
GPIO_PORTF_DIR_R    EQU     0x40025400
GPIO_PORTF_DEN_R    EQU     0x4002551C
GPIO_PORTF_DATA_R   EQU     0x400253FC

        AREA    |.text|, CODE, READONLY, ALIGN = 2
        THUMB 
        EXPORT  Main

Main

        BL GPIOF_INIT

loop    BL LIGHT_ON                 ; hình như viết cùng dòng thế này là để nói loop nằm trong main :))
        B loop

GPIOF_INIT

        ;load địa chỉ thanh ghi RCGCGPIO register vào R1
        LDR R1, =SYSCTL_RCGCGPIO_R

        ;lấy giá trị của thanh ghi RCGCGPIO register ghi vào R0, reset value của RCGCGPIO register là 0x0000000 thì phải :)) 
        LDR R0, [R1]

        ;giờ R0 đã chứa giá trị của thanh ghi RCGCGPIO register, giờ ta set bit 5 lên để tí ghi vào lại thanh ghi RCGCGPIO register, set PortF
        ORR R0,R0,#0x20

        ;ghi giá trị R0 vào nới R1 trỏ tới
        STR R0,[R1]

        LDR R1, =GPIO_PORTF_DIR_R

        ;setbit 2 của thanh ghi
        MOV R0, #0x02
        STR R0,[R1]

        LDR R1, =GPIO_PORTF_DEN_R
        MOV R0, #0x02
        STR R0,[R1]

        BX LR

LIGHT_ON
        LDR R1,= GPIO_PORTF_DATA_R
        MOV R0, #0x02
        STR R0, [R1]
        BX  LR

        ALIGN
        END

```

***Coding BlinkLed with Assembly**

thì bài này nó sẽ giúp ta ứng dụng Assembly thành thạo hơn, nma cơ bản chưa cần thiết, giờ ta nên tập trung đẩy nhanh kiến thức ròi làm project lớn hơn, nên là có dịp sẽ code lại Blink Led Assembly

```s

```

</details>



<details><summary><h1> 25. ARM Cortex-M Bare-Metal Embedded-C Primer</h1></summary>

Cơ bản bài này sẽ debug cho ta xem các lệnh bên trong code của ARM dưới dạng Assembly trông như nào
Ví dụ như ở bài 2+3 chỉ cần xem 2 bài đấy là ta biết phần 25 này là làm gì luôn :)) cho count++, xong debug xem Assembly, xong chắc bài bên dưới cũm dạng kiểu nhìn debug như vậy

Từ bài 6 sẽ bắt đầu nói về CMSIS (Cortex Microcontroller Software Interface Standard) thì đây là 1 chuẩn phần mềm hay có thể là chuẩn API do bên ARM sẳn xuất cho tất cả các dòng Cortex M
Ví dụ như các API của NVIC, SysTick, Interrupt number ... những cái có trong core, ngoài ra đặc biệt CMSIS sẽ chuẩn hóa các địa chỉ của GPIO thành tên. Ví dụ địa chỉ của GPIOA sẽ được #define GPIOA_ADDDRESS_B 0x4000100 chẳng hạn, và người dùng không cần phải define lại, mình chỉ cần đọc lại các file.h mà CMSIS cung cấp.

Xem bài 7 để thấy rõ được các thành phần của CMSIS

Còn từ bài 8 phần coding chính là cái phần lập trình thanh ghi mà ta hay biết :)), tức là nó sẽ lấy các define sẵn của CMSIS để mà code button led

Từ bài 11 đến cuối nói về struct và typedef :))

Oke đến đây là xong khóa ròi.

</details>

<details><summary><h1> 1 số câu lệnh Assembly </summary></h1>

**TST Rn , Operand2**
Ý nghĩa là thực hiện phép AND giữa Rn và Operand2, nhưng không lưu kết quả, thay vào đó, nó chỉ cập nhật các cờ trong thanh ghi trạng thái (Zero flag, Negative flag nằm ở trong thanh ghi PSRx)
Vì TST này dùng để trả về trạng thái 0 và 1 nên nó sẽ được lưu ở zero flag, còn nếu nó âm nó sẽ được lưu ở negative.

**ITE EQ**
If then else đây là 1 block điều kiện
EQ = Equal -> nó sẽ vào check zero flag
ITE EQ tức là EQ = 1 thực hiện câu lệnh ngay bên dưới
Còn EQ = 0 thực hiện câu lệnh dưới nữa
Ngoài ra còn có các ITE NE(Not Equal)-> ktra Zero flag(Z==0), ITE LT(Less Than)-> Ktra Negative flag(N) và Overflow flag(V), ITE GT(Greater Than) -> kiểm tra kết hợp Z,N,V.

</details>

<details><summary><h1> 1 số định nghĩa </summary></h1>

**Nếu làm về Embedded truyền thống thì có thể hiểu được như bên dưới:**
+ Background: là chương trình chạy liên tục thường là hàm main hoặc while(1) nó luôn chạy và có thể bị ngắt bất cứ lúc nào bởi các interrupt.
+ Foreground(Interupt Service Routines -ISR): là các đoạn mã chạy khi có sự kiện ngắt xảy ra.

**Nếu làm về OS, đa luồng, event-driven system**: thì ISR là background cũm hợp lý, mà trong course này ta làm về RTOS nên có thể hiểu background sẽ là những tiến trình ngắt.

**Blocking code:**
+ Nghĩa là đoạn code thực thi sẽ chặn đứng toàn bộ chương trình tại đó cho tới khi nó hoàn thành xong. Nói cách khác thì khi code block, CPU không thực hiện được các câu lệnh tiếp theo mà phải chờ câu lệnh hiện tại thực hiện xong thì mới tiếp tục -> gây trễ, lag, giảm hiệu suất. VD: delay(1000); // đợi 1s này là 1 hàm blocking code.

</details>

# RTOS Update 

<details><summary><h1> 2. Round Robin Scheduler Recap </h1></summary>

***1. Review on basic RTOS concepts**
Ở bài này là review lại RTOS là gì và nói về Classification of Schedulers và TCB.
Thì ở đây ta sẽ nói về Classicfication đi, vì phần này lúc trước mình chưa rõ nhiều cái dù đã ghi.

Classicfication of schedulers sẽ gồm
- Static vs Dynamic
- Preemtive and Non preemtive

đầu tiên ta sẽ nói về static scheduler thì priority của thread đã được set trước khi run, ngược lại với Dynamic scheduler các thread sẽ được update priority trong quá trình chạy. Tức là cái dynamic có thể thay đổi được priority của 1 thread, ví dụ task A có thể đang là priotity 1 thành priority 2. Còn với preemtive bên dưới nó không có khả năng thay đổi priority của 1 thread, nó sẽ là thuật toán mà khi 1 thread đang chạy sẽ cho phép 1 thread khác ngắt nó. Ví dụ thread A đang chạy chưa xong, thì thread B được kích hoạt bởi periodic interrupt (hay là ngắt theo chu kỳ) và thread B sẽ chiếm CPU để chạy, còn thread A phải đợi. Thì preemtive chính là việc cho phép 1 thread khác chiếm CPU trong khi 1 thread đang chạy.

Còn với preemtive scheduling algorithm thì 1 thread có thể bị tạm dừng nhường cho 1 thread periodic hoặc là có 1 thread có priority cao hơn. Còn Non preemtive thì 1 khi thread đó được thực thi nó sẽ chạy cho đến khi nó hoàn thành trừ khi nó tự quyết định nhường quyền cho CPU sử dụng. 

***2-3-4-5-6-7. Coding Round Robin**

Thì ở bài này coding lại Round-Robin giống ở khóa 1 chỉ thay đổi phần Init nhưng mà ta sẽ code lại từ đầu cho quen

### osKernel.h
```h
#ifndef __OS_KERNEL_H__
#define __OS_KERNEL_H__

#include <stdint.h>
#include <stm32f4xx.h>

void osKernelInit(void);
void osKernelLaunch(uint32_t quanta);
uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void));

#endif

```


### osKernel.c
```c
#include "osKernel.h"

#define NUM_OF_THREAD       3
#define STACKSIZZE          100 // 100 word - 400byte

#define BUS_FREQ            16000000

#define SYSPRI3             (*((volatile uint32_t*)0xE000ED20))

uint32_t MILLIS_PRESCALER;

struct tcb{
    uint32_t *stackPt;
    struct tcb *nextPt;
};

typedef struct tcb  tcbType;
tcbType tcbs[NUM_OF_THREAD];
tcbType *currentPt;

uint32_t TCB_STACK[NUM_OF_THREAD][STACKSIZE];
void osSchedulerLaunch();


static void osKernelStackInit(int i){
    tcbs[i].stackPt = TCB_STACK[i][STACKSIZE-16];
    /*thumb mode*/
    TCB_STACK[i][STACKSIZE-1] = 0x01000000;

    /*Init register to easy for debug*/
    /*auto register - tức là những register sẽ tự động được push vào trong register bank của CPU- R0 - R3 LR PC xPSR*/
    /*ở đây ta sẽ cho giá trị rác để dễ debug, vì lần đầu tiên vào hàm thì làm sao có giá trị trước đó, nên dùng gtri rác để nói rằng t đã vào hàm*/
    /*PC là giá trị CPU register sẽ lấy để trả về hàm khi lần đầu hàm vào -> PC register không để giá trị rác*/
    TCB_STACK[i][STACKSIZE-3] = 0x14141414; // R14
    TCB_STACK[i][STACKSIZE-4] = 0x12121212; // R12
    TCB_STACK[i][STACKSIZE-5] = 0x03030303; // R3
    TCB_STACK[i][STACKSIZE-6] = 0x02020202; // R2
    TCB_STACK[i][STACKSIZE-7] = 0x01010101; // R1
    TCB_STACK[i][STACKSIZE-8] = 0x00000000; // R0

    /*manual register - đây là những thanh ghi mình sẽ tự tay push vào ở trong file.s ta sẽ tự push vào register bank*/
    TCB_STACK[i][STACKSIZE-9] =  0x11111111; // R11
    TCB_STACK[i][STACKSIZE-10] = 0x10101010; // R10
    TCB_STACK[i][STACKSIZE-11] = 0x09090909; // R9
    TCB_STACK[i][STACKSIZE-12] = 0x08080808; // R8
    TCB_STACK[i][STACKSIZE-13] = 0x07070707; // R7
    TCB_STACK[i][STACKSIZE-14] = 0x06060606; // R6
    TCB_STACK[i][STACKSIZE-15] = 0x05050505; // R5
    TCB_STACK[i][STACKSIZE-16] = 0x04040404; // R4
}

uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void)){
    __disable_irq();
    //tcb0 -> tcb1 -> tcb2
    tcbs[0].nextPt = &tcbs[1];
    tcbs[1].nextPt = &tcbs[2];
    tcbs[2].nextPt = $tcbs[0];

    osKernelStackInit(0);
    TCB_STACK[0][STACKSIZE-2] = &task0;

    osKernelStackInit(1);
    TCB_STACK[1][STACKSIZE-2] = &task1;

    osKernelStackInit(2);
    TCB_STACK[2][STACKSIZE-2] = &task2;

    currentPt = tcbs[0];

    __enable_irq();
    return 1;

}

void osKernelInit(void){
    MILLIS_PRESCALER = BUS_FREQ / 1000; // 1ms
}
void osKernelLaunch(uint32_t quanta){
    SYSTICK -> CTRl = 0;
    SYSTICK -> VAL = 0;
    SYSTICK -> LOAD = (quanta * MILLIS_PRESCALER) - 1;

    /*set priority for Systick*/
    SYSPRI3 = (SYSPRI3 & 0x00FFFFFF) | 0xE0000000;  // xóa 2byte ở thanh ghi cao, xong set bit 7 -> priority 7
    SYSTICK -> CTRL = 0x00000007;
    osSchedulerLaunch();
    
}

```

### osKernel.s

```s
            AREA    |.text|, CODE, READONLY, ALIGN = 2
            THUMB
            IMPORT currentPt
            EXPORT  SysTick_Handler
            EXPORT  osSchedulerLaunch

SysTick_Handler         ; save R0 R1 R2 R3 R12 LR PC PSR
    CPSID   I
    ; save SP
    PUSH    {R4-R11}
    LDR     R0, =currentPt
    LDR     R1, [R0]
    STR     SP, R1

    ; take SP of next Pt
    LDR     R1, [R1,#4]
    LDR     R0, [R1]        ;STR R1, [R0]
    LDR     SP, R0
    POP     {R4 - R11}
    CPSIE   I
    BX      LR


osSchedulerLaunch
    LDR     R0, = currentPt
    LDR     R1, [R0]
    LDR     SP, R1

    POP     {R4-R11}
    POP     {R3-R0}
    POP     {R12}
    ADD     SP,SP,#4
    POP     {LR}
    ADD     SP,SP,#4
    CPSIE   I
    BX      LR

    ALIGN
    END


```

### main.c

```c

#include "osKernel.h"

#define QUANTA      10

uint32_t count0,count1,count2;

void  Task0(void){
    while(1){
        count0++;
    }
}
void  Task1(void){
    while(1){
        count1++;
    }
}
void  Task2(void){
    while(1){
        count2++;
    }
}

int main(){
    osKernelInit();
    osKernelLaunch(QUANTA);
    osKernelAddThreads(&Task0, &Task1, &Task2);
}

```

***8.Coding Cooperate Scheduers**

Thì như ta biết cái Cooperate này sẽ chủ động nhường CPU cho thread khác nếu chạy xong 1 lần, còn như Round Robin như trên nó sẽ chạy 10ms ròi mới chuyển sang thread khác. Cái này khá rõ ở bài 14 The CooperativeNon-Preemptive Scheduler ở khóa trước ròi, nên phần này khả năng sẽ xem thoi, chủ yếu nó sẽ thêm 1 cái hàm OsKernelYeild, và cái hàm này sẽ kích hoạt Systick sau khi thread chạy xong lần 1 -> khiến thread đó mỗi lần chạy là count++ 1 lần, còn nếu không dung Yeild thì thread đó sẽ chạy 10ms tức là có 1 lần gọi thread có thể count++ 100 lần.

***9. The PendSV Thread Switcher**

Bài này ta sẽ thay SysTick_Handler trong file.s thành PendSV_Handler

Và trong main ta sẽ gọi SysTick_Handler và Systick_Handler sẽ gọi PendSV_Handler ở bên trong tức là bên trong Systick sẽ kích hoạt PendSV thông qua thanh ghi, nó giống như ta kích hoạt SysTick_Handler với Cooperate Scheduers. Với lại kích hoạt SysTick và PendSV lại ở chung thanh ghi :)) , quan trọng giờ ta điều chỉnh priority sao cho PendSV sẽ có priority thấp nhất, còn SysTick có priority cao sẽ thực hiện những công việc quan trọng hơn lên trc.

</details>


<details><summary><h1> 3. Period Scheduling </h1></summary>

***1. Coding  Type 1 Periodic Scheduling**
Oke khóa này khác khóa trước là nó có 4 type :)) và nó không dùng BSP nên ta có thể code
Thì với method đầu tiên là ta sẽ sử dụng 1 biến count. và khi biến count đến 1 thời điểm nhất định nó sẽ kích hoạt thread 
Ví dụ có 3 thread chạy // và time quanta là 10ms, thread period sẽ sử dụng 1 biến count ví dụ như trong bài count = 100, và cứ mỗi 10ms tăng lên 1 -> cứ 1000ms hay 1s thì thread period sẽ được kích hoạt 1 lần.
Giờ cta sẽ triển khai code, thì trước tiên trong file.s ta sẽ thay đổi việc next Pt thay vì ở trong file.s thì sẽ diễn ra trong file.s của period thread. Tức là ở trong period thread ta sẽ so sánh count, nếu count đạt 100 thì nhảy 1 hàm thực thi, còn nếu count < 100 thì sẽ thực hiện chuyển task theo time quanta.

### osKernel.s

```s
            AREA    |.text|, CODE, READONLY, ALIGN = 2
            THUMB
            IMPORT currentPt
            EXPORT  SysTick_Handler
            EXPORT  osSchedulerLaunch

PendSV_Handler         ; save R0 R1 R2 R3 R12 LR PC PSR
    CPSID   I
    ; save SP
    PUSH    {R4-R11}
    LDR     R0, =currentPt
    LDR     R1, [R0]
    STR     SP, R1

    ;push vào R0 là LR vào stack để tránh việc khi nhảy vào hàm bị thay đổi
    ;tại sao lại push 2 cái này?  thì R0 đang nắm đỉa chỉ của currentPt, và sau khi ra khỏi hàm ta sẽ dùng tiếp địa chỉ này, còn LR là nơi trả về
    ;thêm thông tin là khi nhảy vào function thông thường, nó sẽ lưu LR quay về Assembly, nma LR để thoát khỏi PendSV_Handler thì chưa lưu, ta phải lưu LR khi thoát khỏi PendSV_Handler bằng cách thủ công
    PUSH    {R0,LR}
    
    ; take SP of next Pt
    ;LDR     R1, [R1,#4]    ; bình thường ta sẽ dùng cái này để nextPt nhưng giờ ta sẽ nhảy vào hàm ở file.c để làm
    BL osSchedulerRoundRobin

    ;POP R0 và LR từ stack ra, lúc này R0 vẫn = currentPt
    ;trong osSchedulerRoundRobin ta đã thay đổi currentPt sang tcb tiếp theo -> giờ ta sẽ lưu thẳng stackPt vào trong register SP, để contet switching
    POP     {R0, LR}
    LDR     R1, [R0]        ;STR R0, [R1]

    LDR     SP, [R1]
    POP     {R4 - R11}
    CPSIE   I
    BX      LR


```

### osKernel.c
```c

#define PERIOD_T    100

// protypde ghi trong file osKernel.h -> cả main.c và osKernel.c đều dùng được
void PeriodicTask(void);

/*hàm này trong main nhớ*/
void PeriodicTask(void){
    while(1){
        LED_TOGGLE();
    }
}

static uint32_t count = 0;
void osSchedulerRoundRobin(void){
    if(count++ == PERIOD_T){
        /*run task periodic*/
        PeriodicTask();
        count=0;
    }
    /*tro toi dia chi tiep theo*/
    currentPt = currentPt->nextPt;
}
```

***2. Coding  Type 1 Periodic Scheduling  with Multiple Periodic Threads**

Oke thì cái thuật toán (method 1) thì nó vẫn sẽ giống với 1 Periodic nhưng đây sẽ là Multiple Periodic. Về cơ bản file.s nó vẫn sẽ như vậy chỉ có file.c  hàm osSchedulerRoundRobin sẽ xử lý khác là thêm nhiều điều kiện if hơn thoi, khác cái nữa là ta sẽ không cho cái count về 0 luôn, vì tại dùng nhiều task mà. Oke ví dụ luôn cho dễ hiểu

```c

/*
* có 2 periodic: Task 1 thì count 100 thì chạy, Task 2 count = 300 thì chạy
*/

static uint32_t count = 0;
void osSchedulerRoundRobin(void){
    count++;
    /*chia lấy dư, dư 0 tức là chia hết -> chia hết là đến đúng chu kỳ count = 100*/
    if(count%100 == 0){
        /*run task periodic*/
        PeriodicTask1();
    }
    if(count%300 == 0){
        /*run task periodic*/
        PeriodicTask1();
    }
    /*tro toi dia chi tiep theo*/
    currentPt = currentPt->nextPt;
}
```

***3. Coding  Type 2 Periodic Scheduling  Using Hardware Interrupts**

Oke với method này cta sẽ dùng tới timer của hardware, tạo ra ngắt timer, và trong ngắt timer sẽ xử lý thread periodic, cái này cảm giác nó giống sử dụng ngắt hơn là RTOS, vì nó phải tác động vào ngắt. 

Dùng cách này không cần phải thay đổi file.s

đầu tiên phải set up timer và set IRQ timer
### STM32F4_BSP.c

```c
void BSP_TIM2_Init(void){
    RCC-> APB1ENR |= 1;
    TIM2->PSC  = 16000-1; /*16 000 000*/
    /*mỗi chu kỳ là 1s hay cứ 1s sẽ vào TIM2_Handler()*/
    TIM2->ARR  = 1000-1;

    //bắt đầu đếm
    TIM2->CR = 1

    TIM2->DIER |= 1; /*Enable UIE, cho phép sử dụng cờ ngắt, nhưng mà lúc này chưa bật ngắt vì NVIC quản lý bộ ngắt*/
    NVIC_EnableIRQ(TIM2_IRQn); /*bật bộ ngắt -> ngắt sẽ vào TIM2_Handler()*/
}

```

### main.c

```c

void TIM2_IRQHandler(void){
    // clear flag
    TIM2->SR = 0;

    //run preodic task
    LED_BLYNK();
}

```

***4. Coding Type 3 Periodic Scheduling Using Thread Control Blocks**

như cái tên là nó sử dụng TCB cho Periodic Scheduling, oke phân tích về cách này, thì về cơ bản nó có thể thay thế TCB của Round Robin luôn và t cảm giác cách này nó giống thực tế mà những IDE hay làm.

Thứ với Round Robin trước ta có dùng TCB nma Time Quanta của cta bị cố định, tức là các task dùng chung 1 time quanta ví dụ 10ms thì tất cả các task đều chạy 10ms. Vậy giờ ta muốn ví dụ task A chạy 10ms, task B chạy 100ms, task C chạy 50ms, thì cái periodic scheduler bằng TCB nó sẽ làm được như thế. 

Oke phân tích về cái TCB của periodic trong bài này, thì ở trong bài này periodic nó chỉ có 2 member đó là chọn task và time task đó chạy là bao lâu, tức là nó sẽ không có trỏ tới stackPt và nextPt như TCB gốc. Nma giờ ta tưởng tượng giờ ta gộp 4 member đó vào 1 TCB thì chắc chắn nó sẽ rất giống với í tưởng task A chạy 10ms, task B chạy 100ms, task C chạy 50ms. 

Nma ở trong bài này ta sẽ làm quen việc nó set time như thế nào, thì đầu tiên đây là kiểu demo thoi nên số lượng task sẽ cố định nên hàm init cũm sẽ cố định là 2 task.

Ngoài ra trong bài này ta thấy Time Quanta vẫn đang cố định là 10ms là nhảy vào Sytick và PendSV để context switching, thì câu hỏi đặt ra là như vậy có tối ưu nếu mà ta có 2 task và 1 task đến 50ms mới chạy và 1 task 100ms mới chạy. Thì thực tế để time quanta là 10 cũm được vì nó chỉ lãng phí lúc 2 thread không chạy // thoi, chứ lúc chạy // nó sẽ giúp 2 task chạy liên tục. Với lại 10ms chạy vào Systick thì nó cũm có được đổi đâu, vì ta sẽ viết 1 cái hàm để check như kiểu if chẳng hạn, ví dụ như cái if(count%100 == 0) ấy, nếu if đúng thì mới context switching thì nó cũm không tốn time.

Ở method này ta sẽ thay đổi file.s tương tự với method type 1, tức là trong assembly sẽ gọi ra 1 cái hàm để check if

### osKernel.c

```c
#define NUM_PERIODIC_TASKS  2
#define NULL    (void*)0
typedef void(*taskT)(void);

typedef struct{
    taskT task;
    uint32_t period;
}periodicTaskT;

static periodicTaskT PeriodicTasks[NUM_PERIODIC_TASKS];
static uint32_t TimeMsec;   // biến đếm cho period
static uint32_t MaxPeriod;  // tìm max để reset period về 0

/*hàm này nó sẽ cố định là có 2 periodic, muốn không cố định tự tạo hàm khác*/
uint8_t osKernelAddPeriodThreads(void (*thread1)(void), uint32_t period1,void (*thread2)(void), uint32_t period2 ){
    /*tìm ra period lớn nhất của 2 thread*/
    MaxPeriod = period1 > period2 ? period1 : period2;
    uint32_t MinPeriod = period1 < period2 ? period1 : period2;

    /*check xem MaxPeriod có chia hết MinPeriod không, vì các period của các thread nên có bội số chung*/
    if(MaxPeriod % MinPeriod != 0) return 0;
    
    PeriodicTasks[0].task = thread1;
    PeriodicTasks[0].period = period1;
    PeriodicTasks[1].task = thread2;
    PeriodicTasks[1].period = period2;

    return 1;
}



void osSchedulerPeriodicRR(void){

    if(TimeMsec < MaxPeriod) TimeMsec++;
    else TimeMsec = 0;
    
    /*nếu ta sử dụng mutiple thread thì sẽ phải dùng for, với lại ta viết hết địa chỉ các hàm vào struct, nên không cần phải gọi ra thủ công như ở method type 1*/
    for(int i = 0; i< NUM_PERIODIC_TASKS; i++){
        if((TimeMsec % PeriodicTasks[i].period) == 0 && PeriodicTasks[i].task != NULL){
            PeriodicTasks[i].task();
        }
    }
    currentPt = currentPt-> nextPt;
}

```

### main.c

```c

void thread1(){

}
void thread2(){

}

int main(){
    /*đó gọi này trông rất trực quan*/
    osKernelAddPeriodThreads(thread1, 100, thread2, 500);
}

```

***5. Coding Type 4 Periodic Scheduling Using TCB and Hardware Interrupt**


Oke thì í tưởng của cái phần này là trong timer ngắt ta sẽ gọi tới 1 cái hàm, hàm này sẽ -- cái biến count của thread, nên count-- = 0 -> thread đó sẽ được thực thi. Và đương nhiên cái timer ngắt nó sẽ được set time bao nhiêu ms or s sẽ nhảy vào Timer_handler. Mỗi lần nhảy vào timer_handler sẽ count -- cho đến khi count = 0, thì thực thi thread.

Do vì mỗi thread sẽ có 1 biến count -- riêng -> trong TCB sẽ có 1 biến count --

### osKernel.c
```c

#define NUM_PERIODIC_TASKS  5
static periodicTaskT PeriodicTasks[NUM_PERIODIC_TASKS];

typedef struct{
    taskT task;
    uint32_t period;
    uint32_t TimeLeft;
}periodicTaskT;

/*biến dùng để đếm số thread đã khởi tạo, max là 5 vì NUM_PERIODIC_TASKS ta define là 5*/
uint32_t NumberPeriodThreads = 0;

/*Oke giờ t sẽ viết lại cái hàm OsKernelAddThread để cho nó chung nhất chứ không cố định là 2 thread tại bài này ta muốn nó lên 5 thread, mà nếu viết tay 5 thread có mà đi*/

uint8_t osKernelAddPeriodThreads(void (*thread)(void), uint32_t period){
    if(NumberPeriodThreads == NUM_PERIODIC_TASKS || period == 0) return 0;

    PeriodicTasks[NumberPeriodThreads].task = thread;
    PeriodicTasks[NumberPeriodThreads].period = period;
    /*set period ban đầu*/
    PeriodicTasks[NumberPeriodThreads].TimeLeft = period-1;
    NumberPeriodThreads++;
    return 1;
}

/* hàm này sẽ dùng để -- count và nhảy vào thread */
void periodic_events_execute(void){
    /*check all thread*/
    for(int i=  0; i< NumberPeriodThreads; i++){
        if(PeriodicTasks[i].period > 0) PeriodicTasks[i].TimeLeft--;
        else{
            PeriodicTasks[i].task();
            PeriodicTasks[i].TimeLeft = PeriodicTasks[i].period - 1;
        }
    }
}

/*biến toàn cục cho function init của osPeriodTask_Init*/

void (*PeriodicTask)(void);

/*Viết hàm Init timer thì bài này cta sẽ có 3 tham số trong hàm Init gồm
* 1. Function mà Timer này tác động đó là hàm periodic_events_execute, hàm này sẽ -- count và nhảy vào các thread. Nma trong task mới là khai báo chứ chưa thực thi
* 2. Thiết lập tần số nhảy vào Timer_Handler tức là 1ms hay 1s sẽ nhảy vào Handler ấy
* 3. set priority for Timer
*/

static void osPeriodTask_Init(void(*task)(void), uint32_t freq, uint8_t priority){
    PeriodicTask = task;
     RCC-> APB1ENR |= 1;
    TIM2->PSC  = 16-1;
    /*tự config chu kỳ*/
    TIM2->ARR  = (freq-1);

    //bắt đầu đếm
    TIM2->CR = 1

    TIM2->DIER |= 1; /*Enable UIE, cho phép sử dụng cờ ngắt, nhưng mà lúc này chưa bật ngắt vì NVIC quản lý bộ ngắt*/
    NVIC_SetPriority(TIM2_IRQn, priority); /*set priority for timer*/
    NVIC_EnableIRQ(TIM2_IRQn); /*bật bộ ngắt -> ngắt sẽ vào TIM2_Handler()*/
}

/*cứ sau 1 freq là sẽ nhảy vào handler và gọi hàm periodic_events_execute*/

void TIM2_IRQHandler(void){
    TIM2->SR = 0;
    PeriodicTask();

}

/* cho hàm Init Periodic vào Init của Kernel*/

void osKernelInit(void){
    osPeriodTask_Init(periodic_events_execute, 1000, 6);
}

```

### main.c

```c

void thread1(){

}
void thread2(){

}

int main(){
    /*khai báo từng cái 1 nếu muốn thêm thread perioridic, không add không thread chạy. còn không chỉ cần Kernel Init là chạy hết code bên trên*/
    osKernelAddPeriodThreads(thread1, 10);
    osKernelAddPeriodThreads(thread2, 100);

    osKernelInit();
}

```




</details>

<details><summary><h1> 4. Board Support Package (BSP) </h1></summary>

***1-2-3-7. Coding GPIO**
Thì cái GPIO này ta code nhiều ròi nên ta sẽ code những cái bên dưới đây

bài này ta set input GPIO cũm được này là ở  bài 7

```c
#define GPIOA_CLOCK (1<<0)
#define BSP_Button_PORT     GPIOA

void BSP_Button_Init(void){
    RCC -> AHB1ENR |= GPIOA_CLOCK; 
    BSP_Button_PORT-> MORDER &= ~0x00000011 /*Clear PA0 pin*/
}

uint32_t BSP_Button_Read(void){
    return BSP_Button_PORT-> IDR & 0x01;
}

int main(){
    BSP_Button_Init();
    while(1){
        if(BSP_Button_Read() & 0x01){
            LED_ON();
        }
    }
}

```

***4-5.Coding ADC BSP**

Thì phân tích 1 chút thì đầu tiên ADC nó là đầu vào input của 1 pin -> ta vẫn sẽ phải set GPIO
Ngoài ra ta còn phải cấp clock cho chân GPIO, và cho bộ chuyển đổi ADC
Xong ròi ta sẽ đi cấu hình cho ADC thì thanh ghi CR2 là thanh ghi cấu hình ví dụ như ở bit 1 là ADON sẽ bật tắt ADC
Và ADC có nhiều channel tại ta mới chỉ bật clock của GPIO A và ADC1 chứ chưa nói rõ chân nào được sử dụng thì ở đây ta sẽ sử dụng chân channel 1 -> vào thanh ghi SQR3 để set channel
và ta muốn set số lượng chân sẽ chuyển đổi thì ta sẽ sử dụng SQR1 với thanh ghi L + 1 -> ở trong bài ta sẽ set = 0 thì nó sẽ lên 1, tức là sẽ có 1 channel chuyển đổi
Lưu ý ở nếu ta xem thanh ghi SQR1 thì thì từ bit 0 - bit 19 nó sẽ coi là 1 conversion, còn nếu muốn set nhiều conversion thì sẽ set ở bit 20

Ncl nên xem các thanh ghi để hiểu rõ hơn tại có thanh ghi là trường bit, có thanh ghi chỉ có các bit thoi.

Đó thì tiếp theo muốn bắt đầu quá trình đọc channel chuyển đổi thì ta lại vào thanh ghi CR2 set start conversion lên -> nó sẽ bắt đầu đọc và chuyển đổi, cta sẽ chờ nó chuyển đổi xong để lấy data

Đương nhiên ADC sẽ còn nhiều mode khác như đọc pin liên tục nma ở bài này ta sẽ đọc 1 lần, và như trong video là ta đoc thông qua while(1) để đọc liên tục, chứ không set mode contiunous.

### STM32F4_RTOS_BSP

```c

void BSP_ADC1_Init(){
    /*GPIO Pin*/
    RCC->AHB1ENR |= 1;      /*ENABLE clock for GPIOA*/
    GPIOA->MODER |= 0xc;    /*PA1 set as analog*/

    /*Setup ADC - Clock ADC*/
    RCC->APB2ENR |= 0x00000100;

    ADC1->CR2 = 0;
    ADC1->SQR3 = 1; /*conversion sequence start at channel 1*/
    ADC1->SQR1 = 0; /*length of sequence is 1*/

    ADC1->CR2 = 1; /*enable ADC - tức là cho phép dùng ADC sẵn sàng để chạy*/
}

uint32_t BSP_Sensor_Read(void){
    ADC1-> CR2 |= 0x40000000; /*Start conversion*/
    while(!(ADC1->SR & 2)){} /*wait for conversion complete*/
    return ADC1 -> DR;
}

```

***6.Coding TIMER BSP**


```c
void BSP_Delay_Milisecond(uint32_t delay){

    RCC-> AHB1ENR |= 0x02; /*Enable TIM3 clock*/
    TIM3 -> PSC = 160 - 1 /* 16 000 000 / 160 = 100 000*/
    TIM3 -> ARR = 100 - 1 /* 100 000 / 100 = 1000*/
    TIM3 -> CNT = 0;
    TIM3 -> CR1 = 1; 

    for(int i = 0; i< delay; i++){
        while(!(TIM3->SR & 1)){} // chờ cho cái bit nay set lên tức là CNT = ARR, tại ta code kiểu cho CNT = 0 mỗi lần vào hàm
        TIM3 -> SR &= ~1;
    }
}

```

***8. Coding Setting Up Probes**

Bài này cách set up các chân không khác gì GPIO, nó chỉ khác đảo chân để mình đo tần số

</details>


<details><summary><h1> 5. Inter-Thread Communication</h1></summary>

***1. Semaphores Recap**

Bài đầu nói lại về Semaphore là gì, thì ta có thể đọc lại tài liệu đã ghi ở khóa 1

***2. Coding   Creating a SpinLock Semaphore**
Thì ở khóa trước t không có code, nhưng khóa này t sẽ code

Cơ bản Semaphore hình thành bởi 3 function Init, SignalSet and SignalWait

Với lại cta để ý thì semaphore không để block periodic thread, bởi vì là periodic thường được xuất hiện cố định là các task quan trọng, nhưng mà thực ra cũm không hẳn, cơ bản là vì semaphore cơ chế là để điều tiết các thread ròi, thread nào được gọi thì sẽ chui ra, chứ không hẳn là cần quan trọng, mà đơn giản là chui ra khi cần thiết sẽ đỡ tốn CPU.

### osKernel.c

```c

/*Init semaphore*/
void osSemaphoreInit(uint32_t *semaphore, uint32_t value){

    *semaphore = value;
}

void osSignalSet(uint32_t *semaphore){
    __disable_irq();
    *semaphore += 1;
    __enable_irq();
}
void osSignalWait(uint32_t *semaphore){
    while(*semaphore <= 0){
        __disable_irq();
        __enable_irq();
    }
    __disable_irq();
    *semaphore -= 1;
    __enable_irq();
}

```

### main.c

oke thì nói về cơ chế 1 chút thì với việc semphore2 được set = 0 ngay từ đầu -> làm cho count2++ không thể chạy, vì lúc này nó đang bị dính 1 while loop trong osSignalWait, còn với semphore1 thì count1++ chỉ chạy được 1 lần sau đó semphore1 = 0 -> lại dính while loop giống task 2. Còn với task3 không bị dính gì nên sẽ tăng liên tục, oke giờ để task 1 và task 2 có thêm semaphore để mà dùng, thì ta phải dùng osSignalSet để mà tăng semphore lên. Ví dụ trong task 3 ta sẽ ghi thêm osSignalSet
, thì lúc này 2 task 1 và 2 mới count++ mạnh

```c

uint32_t semphore1, semphore2; 


void task1(void){
    while(1){
        osSignalWait(&semphore1);
        count1++;
    }
}
void task2(void){
    while(1){
        osSignalWait(&semphore2);
        count2++;
    }
}

void task3(void){
    while(1){
        count3++;

        osSignalSet(&semphore1);
        osSignalSet(&semphore2);
    }
}


int main(){
    osSemaphoreInit(&semphore1,1);
    osSemaphoreInit(&semphore2,0);
    
}

```

***3. Coding  Using Semaphores for Mutually Exclusive LCD Access**

Oke về cơ bản bài này chỉ xem thoi vì nó khá giống với bài của khóa trước, thì nó sử dụng LCD và khi mình không dùng semaphore thì tài nguyên chung LCD bị conflict, vậy nên ta phải dùng semaphore để đồng bộ tài nguyên chung, tức là task0 dùng xong ròi mới đến task1 dùng. Chứ task0 đang dùng dở mà đến lượt task1 dùng luôn sẽ bị lỗi. Ncl t đã nói khá chỉ tiết ở khóa trước. Đây thì ta sẽ copy lại thì nó cũm sẽ giải thích code trong video của bài này luôn (ta có thể mở video ra)

Thông qua video phần 7 Part 4 ấy thì ta có thể thấy cái Spin-lock này nó như mutex vậy :)) Thì xem video có thể thấy cả 2 task đang dùng chung 1 cái tài nguyên LCD, nếu ta không dùng semaphore thì ví dụ task A nó đang dùng giở thì phải chuyển sang task B và lúc này task B cũm sẽ truy cập vào LCD -> nó sẽ bị xung đột. Vậy nên ta phải dùng 2 semphore, theo như video thì task2 nó sẽ được thực thi trước, nó sẽ SignalWait() semaphore1 vì semaphore1 = 1; (chạy xong hàm Signalwait() semaphore1 sẽ = 0) và khi chạy xong hết lệnh LCD thì task2 sẽ signalSet() semphore2 lúc này semaphore2 sẽ từ 0 lên 1 -> task1 dùng semphore2 nên lúc này có thể chạy, còn trước đó với việc semphore2 = 0, nên dù có nhảy vào task1 thì lệnh LCD cũm không được chạy.

***4. Coding Synchronizing Tasks using Semaphores (Rendezvous)**
Oke thực ra về thuật toán Rendezvous ta cũm nói khá rõ và code ở khóa trước bài 20.Inter-Thread Communication And Synchronization ròi, nên ở phần này ta chỉ xem video xem có gì mới không.
Cơ bản nó không có gì mới mà lại giải thích khó hiểu hơn -> nếu muốn hiểu xem lại khóa cũ hay hơn.

***5. Coding  Creating a Cooperative Semaphore**
Oke thì cái Cooperative này nó là cái osThreadYeild ấy :)) là cái thread đó sẽ chủ động nhường thay vì chờ 10ms, ncl cái này t cũm nói ròi, ở khóa trước, nên là bài này lại xem tiếp. 
Thì cái này là ta sẽ viết hàm osThreadYeild vào trong cái while của osSignalWait, thì thay vì ta cứ while trong 10ms thì ta chuyển sang task khác cho nó thực thi

```c
void osSignalWait(uint32_t *semaphore){
    while(*semaphore <= 0){
        __disable_irq();
        osThreadYeild();
        __enable_irq();
    }
    __disable_irq();
    *semaphore -= 1;
    __enable_irq();
}


```


***6. Coding Creating the osThreadSleep API**

Oke bài đáng xem nhất đây :)) osThreadSleep.

Đầu tiên ta sẽ thêm 1 member vào tcb để quản lý việc thread đấy thức hay ngủ, thì sau khi thêm thông số ta cần phải init nó, và cta sẽ init ở trong osKernelAddThreads khi cho 3 cái tcbs[0].sleepTime đều bằng 0, tức là cho nó wakeup ấy

Thì í tưởng ở bài này thì cơ bản chỉ cần sleepTime khác 0 là task đó đang sleep. Oke với việc ta có 1 hàm count-- trước đó ở bài periodic, thì ta sẽ tận dụng nó để ta  --sleepTime cho đến khi sleepTime = 0 thì thread đó sẽ wake. Thì đó là hàm periodic_events_execute, nó dùng để --TimeLeft để chạy các thread period. Nma bài này ta sẽ tận dụng hàm này để --sleepTime. Tức là cứ mỗi freq (10ms 100ms ... cái này do ta setup) là nó sẽ nhảy vào TIM2_Handler và chạy hàm periodic_events_execute(), sau đó nó sẽ --TimeLeft và --sleepTime. Nma nếu mà chương trình không yêu cầu Periodic Thread thì đương nhiên ta có thể xóa vòng for của period thread. Thì lúc này Timer_handler chỉ quản lý mỗi --sleepTime. Với sleepTime = 0 -> thread tương ứng sẽ chạy, và mỗi 1 thread có 1 sleepTime riêng.

Oke tiếp đến là quá trình context switching. Việc thread này nó là sử time quanta cứ 10ms vào 1 lần, thì ta cần 1 điều kiện if để check xem thread này có được switching hay không. Đương nhiên ta sẽ dựa vào sleepTime để check, thì ta sẽ context switching trong hàm osSchedulerRRWithSleep() ta sẽ ghi bên dưới.

Oke chúng ta có thể chủ động làm 1 thread Sleep nhớ, bằng việc set sleepTime lên thì Thread đó Sleep thoi. Thì như trong video là nó sẽ tự làm bản thân Sleep và với việc tự làm bản thân Sleep thì nó sẽ nhảy sang thread khác luôn, không thực hiện Thread đó nữa. Oke đến đây là video hướng dẫn xong, giờ ta chỉ cần tương tác với các function trong main.c

Oke thì với việc thread có thể tự Sleep bản thân, thì ta cũm có thể dùng Thread khác wake up thread này, chứ không nhất thiết phải như bài là dùng sleepTime-- trong Timer_Handler


Code này không phải code chạy luôn tại còn thiếu Init Timer các kiểu, code này chỉ để chứng minh osThreadSleep
### osKernel.c
```c
struct tcb{
    uint32_t *stackPt;
    struct tcb *nextPt;
    uint32_t sleepTime; // sleepTime = 0 mean wakeup
};

uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void)){
    __disable_irq();
    //tcb0 -> tcb1 -> tcb2
    tcbs[0].nextPt = &tcbs[1];
    tcbs[1].nextPt = &tcbs[2];
    tcbs[2].nextPt = $tcbs[0];

    /*Init sleepTime = 0*/
    for(int i = 0; i < NUM_OF_THREADS; i++){
        tcbs[i]= 0;
    }

    osKernelStackInit(0);
    TCB_STACK[0][STACKSIZE-2] = &task0;

    osKernelStackInit(1);
    TCB_STACK[1][STACKSIZE-2] = &task1;

    osKernelStackInit(2);
    TCB_STACK[2][STACKSIZE-2] = &task2;

    currentPt = tcbs[0];

    __enable_irq();
    return 1;
}

void periodic_events_execute(void){
    /*check all normal thread*/
    for(int i = 0; i< NUM_OF_THREADS; i++){
        if(tcbs[i].sleepTime > 0){
            tcbs[i].sleepTime--;
        }
    }
}

/* cơ bản trong TIM2_Handler sẽ chạy periodic_events_execute để -- đi, và đương nhiên Timer vào ngắt là do mình chọn*/
void TIM2_Handler(){
    periodic_events_execute();
}


void osSchedulerRRWithSleep(void){
    /*nó vừa chạy xong thread này ròi, giờ nó sẽ phải check xem thread tiếp theo có được phép chạy hay không*/
    currentPt = currentPt->nextPt;

    /*check xem cái thread tiếp theo nó có sleep hay không, nếu có sleep thì lại nhảy sang check thread tiếp theo nữa mà nếu thread tiếp theo nữa vẫn sleep thì ta cứ while sang thread tiếp nữa nữa cho đến khi tìm được 1 thread không sleep hay sleep = 0 thì ta sẽ context switching cái thread đó*/
    while(currentPt->sleepTime > 0){
        currentPt = currentPt->nextPt;
    }
}

/*function thread sleep - function áp dụng cho chính tự thread hiện tại gọi -> nên gọi function này ở cuối thread*/
void osThreadSleep(uint32_t sleep_time){
    __disable_irq();
    currentPt -> sleepTime = sleep_time;
    __enable_irq();

    /*kích hoạt Systick để nhảy vào PendSV và thực hiện context switching*/
    osThreadYeild();
}
```

### main.c

```c

/*tức là cứ 600 lần vào TIM_Handler nó sẽ chạy Task A 1 lần, mỗi lần vào task A là mỗi lần đảo led ví dụ nó đang 0 thì thành, nó đang 1 thành 0
* ncl cứ 600 lần vào TIM_Handler nó sẽ chạy Task A 1 lần
*/
void TaskA (void){
    while(1){
        LED_TOGGLE();
        osThreadSleep(600);
    }
}

int main(){

}

```

</details>

<details><summary><h1> 6. Inter-Thread Data Transfer</h1></summary>

Đây có thể là 2 bài nói về cách truyền dữ liệu giữa thread với thread

***1. Coding  Sending Data between Threads using Mailboxes**

Oke thì theo giới thiệu thì Mailboxes là gửi data từ 1 thread đến 1 thread khác, thì cơ bản đây là thuật toán sẽ đảm bảo việc truyền nhận data giữa các thread
Oke logic rất đơn gian là cta có 1 hàm gửi data, và chúng ta có 1 biến global để chứa data :)), và cta lại thông qua hàm để cho thread khác nhận được cái data đấy, mặc dù nó là biến global :)), nma cơ bản để tạo ra cơ chế kiểu có cấu trúc thì ngta sẽ vậy.

Oke vậy ta có 2 hàm osMailboxSend và osMailboxRecv, 1 cái gửi 1 cái nhận
Bên gửi sẽ check xem cái buffer data hay MB_data có dữ liệu hay không nếu có thì để bên nhận, nhận được dữ liệu đã, thì mới gửi tiếp dữ liệu mới. Vậy làm sao để biết bên nhận đã nhận dữ liệu thì ta sẽ thông qua 1 biến global MB_hasdata, nếu nó = 1 tức là có dữ liệu = 0 tức là không có dữ liệu. Nma cái này lại còn thêm semaphore để quản lý :)) ncl rất là phức tạp. Bên gửi sẽ kích hoạt sema lên, khi mà sema được kích hoạt thì bên osMailboxRecv mới chạy được các task vụ nhận, vì osSignalWait đứng ở đầu hàm -> nếu mà không có dữ liệu MB_Sem = 0, thì nó sẽ bị đứng im trong hàm osSignalWait tức là while liên tục hoặc là ta có thể dùng osSignalWait phiên bản có Yeild để thoát khỏi hàm sớm hơn, vì code bên dưới có chạy đâu để mất time. Vậy nên cơ bản semaphore ở đây giúp chúng ta đỡ mất time chạy lại các dòng bên dưới của osSignalWait, nếu mà bên gửi đang không có data. Và vì có semaphore làm ta cảm giác MB_hasdata nó hơi bị thừa :)), tại trên thực tế ta check biến MB_Sem cũm được, nma khả năng thầy muốn ta tách biệt rõ ràng việc biến sema chỉ dùng cho ứng dụng semaphore thoi, còn nếu sử dụng if thế kia hoặc để thông báo trạng thái thì cứ dùng biến bình thường.

Với lại cái này nó gửi được mỗi 1 dữ liệu trong 1 lần gửi, tức là nếu mà thread kia không nhận dữ liệu thì bên gửi cũm không thêm được dữ liệu vào. Ví dụ trong lúc bên kia đang nhận, thì có dữ liệu mới thì sao ? Xong kiểu cta chưa kịp cho MB_hasdata về 0 thì tức là MB_data vẫn chưa được phép nhận dữ liệu mới -> ta có thể miss data.

### osKernel.c
```c
static uint8_t MB_hasdata;  // check có data không
static uint32_t MB_data;    // chứa data thực sự
static uint32_t MB_Sem;      // còn đây là semaphore của MB, nó sẽ nói là có bao nhiêu data.

void osMailboxInit(void){
    MB_hasdata = 0;
    MB_data = 0;
    osSemaphoreInit(&MB_Sem,0);
}

/*cái này là 1 Thread gửi đi*/
void osMailboxSend(uint32_t data){
    __disable_irq();
    /*đảm bảo hasdata đang không có, nếu có thoát khỏi hàm*/
    if(MB_hasdata){
        __enable_irq();
        return;
    }
    MB_data = data;
    MB_hasdata = 1;
    __enable_irq();

    osSignalSet(&MB_Sem);
}

uint32_t osMailboxRecv(void){
    osSignalWait(&MB_Sem);
    uint32_t data;

    __disable_irq();
    data = MB_data;
    MB_hasdata = 0;
    __enable_irq();
    return data;
}

```

cái này đơn giản thoi, 1 bên gửi 1 bên nhận :)) bên nhận muốn nhận data thì phải chắc chắn có dữ liệu vì ta dùng cả MB_hasdata để check dữ liệu và dùng cả semaphore osSignalWait để check xem có dữ liệu không. Tức là sẽ có 2 lớp để check xem có dữ liệu hay không nên là gần như chắc chắn sẽ nhận được dữ liệu.

### main.c

```c

uint32_t sensor_value = 4848;

void TaskA(void){
    while(1){
        osMailboxSend(sensor_value);
    }
}

uint32_t recv_value;
void TaskB(void){
    while(1){
        recv_value = osMailboxRecv();
    }
}

int main(){
    osMailboxInit();
}

```

***2. Coding  Sending Data between Threads using FIFO Queues**

Cái này khả năng sẽ tối ưu hơn cái trên :)), về cơ bản nó cũm có 3 cái func giống như thuật toán trên gồm Init, Push vào FIFO, và Take ra khỏi FIFO.

Ngoài ra ta xem thuật toán bên trong là nó sẽ khá là giống queue
Oke nói về PutI và GetI, thì ở đây là ta dùng FIFO first in first out, tức là dữ liệu vào trước thì ra trước.
Ví dụ PutI sẽ nắm giữ vị trí của dữ liệu mới nhất, ví dụ nó sẽ push từ vị trí 0 đến 1 đến 2 ... đến 8 chẳng hạn, thì lúc này PutI = 8. Và lúc này GetI bắt đầu lấy dữ liệu thì GetI nó sẽ lấy từ 0 trước ròi lần lượt mới đến 8 -> bắt buộc phải có PutI và GetI, 1 cái nói đến vị trị dữ liệu gần nhất, 1 cái nói đến vị trí dữ liệu lấy ra.
Ngoài ra ta làm queue thì ta sẽ biết ví dụ PutI chạm mốc 15 tức là max, thì nó sẽ quay lại từ đầu tức là từ 0 để push tiếp data (trong trường hợp các dãy đầu đã được get ra)


```c
#define FIFO_SIZE   15

uint32_t PutI; // push index - đây có thể coi là người đẩy dữ liệu vào - vào trước ra trước First In First out
uint32_t GetI; // get index - đây có thể là coi người lấy dữ liệu ra 
uint32_t OS_Fifo[FIFO_SIZE]; // tạo ra 1 cái fifo thoi
uint32_t current_fio_size;   // đây là biến check kích thước của FIFO, cta lấy nó làm biến cho semaphore luôn :))

uint32_t loss_data;         // optional check xem bao nhiêu data bị mất, tức là khi current_fio_size = MAX tức là không thể ghi thêm dữ liệu, nhưng mà vẫn có dữ liệu đẩy vào -> bị mất data.


void osFIFOInit(void){
    PutI = 0;
    GetI = 0;
    osSemaphoreInit(&current_fio_size,0);
    lost_data = 0;
}

uint8_t osFIFOPut(uint32_t data){
    if(current_fio_size == FIFO_SIZE){
        loss_data++;
        return -1;
    }
    OS_Fifo[PutI] = data;
    /*khi nó PUT đến dãy max nó sẽ quay về đầu để push tiếp*/
    PutI = (PutI + 1) %FIFO_SIZE;
    osSignalSet(&current_fio_size);

    return 1;
}

uint32_t osFIFOGet(void){
    uint32_t data;
    osSignalWait(current_fio_size);
    data = OS_Fifo[GetI];
    GetI = (GetI + 1)%FIFO_SIZE;
    return data;
}

```

Ở đây ta định cho Task A check ADC, còn Task B là lấy dữ liệu đó. Ncl xem video để thấy debug trực quan 

### main.c

```c
uint32_t sensor_value = ADC_Sensor;

void TaskA(void){
    while(1){
        osFIFOPut(ADC_Sensor);
    }
}

uint32_t recv_value;
void TaskB(void){
    while(1){
        recv_value = osFIFOGet();
    }
}

int main(){
    osFIFOInit();
}

```

</details>

<details><summary><h1> 7. Priority Scheduling</h1></summary>

Oke bài này là sẽ chọn thread nào có priority cao nhất sẽ run trước, priority cao nhất là priotity có số nhỏ nhất ví dụ task có priorty = 1 sẽ được chạy trước task có priority = 5

Oke thì nói về thuật toán ở hàm osPriorityScheduler() của bài này nó sẽ check priority của các thread thì priority nào thấp nhất sẽ được chạy, xong ta check sleepTime and blocked xem thread đó có đang bị giam, nếu bị giam thì không lấy. Cái đáng chú ý trong thuật toán là _currentPt->priority < highestPriority, tức là nếu viết như này thì cùng priority sẽ không được chạy. Ví dụ như task1 có priotity là 1 và task 2 cũm vậy. Nma ta chỉ cho điều kiện là < highestPriority, tức là task 2 phải = 0 thì mới được chạy, tức là theo lí thuyết phải là <= highestPriority, thì các task cùng priority sẽ cùng chạy. Nma nếu ta xem video dù vẫn viết < highestPriority thì nó vẫn chạy như thường :)). Vậy chuyện gì đã xảy ra?

Oke thì ta chú ý cái _currentPt = _currentPt->nextPt; và highestPriority = 255; Oke ta sẽ nói từ đầu thì lúc đầu nó ở task 0 và ví task 0 vào đầu tiên nó chắc chắn sẽ count. Và tiếp theo khi vào hàm nó sẽ duyệt và check highestPriority = 1, chính là task 1 lúc này _nextThreadRun = task 1, lúc này ta sẽ duyệt tiếp task 2, thì task 2 cũm có priority là 1, nma điều kiện priority phải < highestPriority và highestPriority lúc này đang = 1 -> sẽ không nhảy vào if. Oke nó cứ duyệt như vậy cho đến khi _currentPt = current tức là nó đã check 1 vòng ròi. Thì lúc này nó sẽ chỉ chạy mỗi task 1. Lúc này ta sẽ context switching sang task 1.
Oke vậy chuyện gì xảy ra tiếp theo, thì khi thực hiện xong task 1 ròi , nó lại nhảy vào để chuyển task, thì ta sẽ lưu í lúc này highestPriority khi vào hàm nó sẽ được làm mới là 255 :)) và _currentPt sẽ check _currentPt->nextPt luôn, vậy nghĩa là task 1 sẽ không được check lại nữa. Và với việc highestPriority là biến local và mỗi lần vào lại 255, thì tức là mỗi lần vào nó sẽ tìm ra priority thấp nhất tiếp. Lúc này Task2 sẽ khởi đầu cho vòng lặp mới và kết thúc ở task 1, thì task 2 priority đã là 1 ròi (là priority thấp nhất ròi) nên các task tiếp theo dù có = 1 cũm sẽ không vào hàm if được vì điều kiện là < highestPriority (tức là muốn lấy context của task 2 thì priority của các task khác phải bằng 0). Còn nếu ta để highestPriority là biến global thì sao :)), thì lúc này highestPriority luôn = 1. Điều đó khiến khi vào task 2 nó sẽ bị check luôn là 1<1 chứ không phải 1<255 nữa. Và điều đó có thể dẫn đến task1 sẽ chạy mãi không cho task khác chạy dù cùng priority.

Oke vậy nếu task1 có priority là 0 thì sao ? Thì đương nhiên task 1 sẽ chiếm thời ginan chạy hết của toàn bộ chương trình. Tại sang task 2 đúng là nó sẽ set highestPriority = 1, nhưng mà khi nó đi 1 vòng quay lại check priority của task 1 thì lúc này 0 < 1. -> task 1 sẽ luôn chạy.

### osKernel.c

```c
struct tcb{
    uint32_t *stackPt;
    struct tcb *nextPt;
    uint32_t sleepTime; // sleepTime = 0 mean wakeup
    uint32_t blocked;   // cập nhật trạng thái của thread
    uint32_t priority;
};

uint8_t osKernelAddThreads(void(*Task0)(void), uint32_t p0,
                            void(*Task1)(void), uint32_t p1,
                            void(*Task2)(void), uint32_t p2,
                            void(*Task3)(void), uint32_t p3,
                            void(*Task4)(void), uint32_t p4,
                            void(*Task5)(void), uint32_t p5,
                            void(*Task6)(void), uint32_t p6,
                            void(*Task7)(void), uint32_t p7,)
{


    osKernelStackInit(0);
    TCB_STACK[0][STACKSIZE - 2] = &Task0;
    
    osKernelStackInit(1);
    TCB_STACK[1][STACKSIZE - 2] = &Task1;

    osKernelStackInit(2);
    TCB_STACK[2][STACKSIZE - 2] = &Task2;

    osKernelStackInit(3);
    TCB_STACK[3][STACKSIZE - 2] = &Task3;

    osKernelStackInit(4);
    TCB_STACK[4][STACKSIZE - 2] = &Task4;

    osKernelStackInit(5);
    TCB_STACK[5][STACKSIZE - 2] = &Task5;

    osKernelStackInit(6);
    TCB_STACK[6][STACKSIZE - 2] = &Task0;

    osKernelStackInit(7);
    TCB_STACK[7][STACKSIZE - 2] = &Task7;
    

    tcbs[0].nextPt = &tcbs[1];
    tcbs[1].nextPt = &tcbs[2];
    tcbs[2].nextPt = &tcbs[3];
    tcbs[3].nextPt = &tcbs[4];
    tcbs[4].nextPt = &tcbs[5];
    tcbs[5].nextPt = &tcbs[6];
    tcbs[6].nextPt = &tcbs[7];
    tcbs[7].nextPt = &tcbs[0];

    for(int i=0; i<= NUM_OF_THREADS; i++){
        tcbs[i].sleepTime = 0;
        tcbs[i].blocked = 0;
    }

    currentPt = tcbs[0];
    return 1;
}

void osPriorityScheduler(void){
    tcbType *_currentPt = currentPt;
    tcbType *_nextThreadRun = _currentPt;
    uint32_t highestPriority = 255;
    do{
        _currentPt = _currentPt->nextPt;
        if(_currentPt->priority < highestPriority && _currentPt->sleepTime == 0 &&  _currentPt->blocked == 0){
            highestPriority = _currentPt->priority;
            _nextThreadRun = _currentPt;
        }
    }while(_currentPt != currentPt);

    currentPt = _nextThreadRun;
}

```


### main.c

```c

void task0(void){
    while(1){
        count0++;
    }
}
void task1(void){
    while(1){
        count1++;
    }
}
void task2(void){
    while(1){
        count3++;
    }
}
void task4(void){
    while(1){
        count4++;
    }
}
void task5(void){
    while(1){
        count5++;
    }
}
void task6(void){
    while(1){
        count6++;
    }
}
void task7(void){
    while(1){
        count7++;
    }
}

int main(){
    osKernelAddThreads(task0, 5, task1, 1, task2, 1, task3, 4, ..., task7, 1);
}

```
</details>

<details><summary><h1> 8. Sporadic Scheduling</h1></summary>

Ở bài 8 của khóa trước ta cũm có nói về khái niệm này ròi

Sporadic Thread: Thì cái Sporadic Thread thường sẽ không có chu kỳ rõ ràng, chạy khi có sự kiện xảy ra, như ngắt do người dùng, hoặc lỗi hệ thống nhưng cái này nó sẽ bị giới hạn về tần suất thực thi. Tức là ví dụ m có 1 nút nhấn ngắt để nhảy vào 1 hàm Sporadic Thread chẳng hạn thì cái này nó sẽ giới hạn như 1s m mới được nhảy vào 1 lần. Tức là dưới 1s m nhấn thỏa mái nó cũm chả thực hiện lại. Giống như việc count++ thay vì ấn liên tục tăng liên tục thì 1s sau ấn mới có thể tăng.

Oke như trong bài cũm nói là ta sẽ kích hoạt 1 chân pin và nó sẽ nhảy vào sporadic thread.
Oke nói về code bên dưới thì nó khá là đơn giản thì về cơ bản ta sẽ sử dụng semaphore để giam thread lại nếu sema = 0 thì đương nhiên thread đó sẽ không chạy được. Và ta sẽ set semaphore bằng cách sử dụng 1 chân PIN, và khi chân PIN đó được trigger thì tức là sema++ và cái thread đó sẽ được chạy. Đơn giản sẽ là thế thoi

### BSP.c

```c
/*enable PC 13 for edge - detection*/
void BSP_EdgeTrigger_Init(){
    RCC-> AHB1ENR |= 4;

    /*Enable clock cho SYSCFG*/
    RCC-> APB2ENR |= 0x4000;

    GPIOC->MODER &= ~0x0C000000;    /*Clear pin mode to input mode tức là 2 bit là 00 thì sẽ được coi là input*/

    /*Enable IRQ cho GPIO*/
    SYCSFG->EXTICR[3] &= ~0x00F0; /*Clear port selection for EXTI13*/
    SYCSFG->EXTICR[3] |= 0x0020; /*Select port C for EXTI13*/

    /*Config cho EXTI*/
    EXTI -> IMR |= 0x2000; /*unmask EXTI13*/
    EXTI -> FTSR |= 0x2000 /*select falling edge trigger*/

    NVIC_EnableIRQ(EXTI15_10_IRQn);

}

```

### osKernel.c

```c

uint32_t *edgeSemaphore;

/*hàm init thoi */
void osEdgeTriggerInit(uint32 *semaphore){
    edgeSemaphore = semaphore; // cái này chắc có nhiệm vụ bao h dùng trong debug :)) chứ nó không ảnh hưởng đến logic của bài
    BSP_EdgeTrigger_Init(); 
}


/*các hàm này và biến này được ghi trong main nhưng mà ta ghi ở đây cho dễ quan sát*/

uint32_t edgeSema;

void SporadicTask(void){
    while(1){
        osSingnalWait(&edgeSema);
        count++;
    }
}

int main(){
    osEdgeTriggerInit(edgeSema);
}
void EXTI15_10_IRQHandler(void){
    osSingnalSet(&edgeSema);
    /*clear flag*/
    EXTI->PR = 0x2000;
}

```

</details>

<details><summary><h1> 9. Fixed Scheduling</h1></summary>

***1. Coding Developing a UART Driver for monitoring results**

Phần này chắc code driver cho UART quá :(( oke thì phần này khá hay :)) nhìn uart ngta code khá clean, hiện thị qua terminal của Laptop luôn mà :)) ngon vc, thì nếu bao h làm uart có thể vào đây xem. Giờ tập trung vào RTOS trước

***2-3. Coding   Building a Finite State Machine**
Oke thì thuật toán bài này nó khá giống cái First Come First Serve Scheduling của bài 10 khóa trước, thì ta nói rất rõ khái niệm trong đây, và xem video thì càng rõ hơn. Thì cơ bản ta sẽ cho các task chạy lần lượt theo time. Tức là lúc 0ms thì task A chạy, lúc 2ms thì taskA dừng taskB chạy kiểu kiểu như vậy ... Thì ngoài ra ta còn có thể viết queue cho các task.

Thì với code ở bài này ta thấy các task sẽ kiểu đến 1 thời điểm thì task này dừng thì đến task khác -> không có trường hợp các task chạy // -> bài này sẽ không có context switching. Nhưng mà trên thực tế ở các ứng dụng khác thì các task chạy // khá nhiều -> lúc ấy sẽ phải context switching.

Oke bài này có 3 hàm thì mỗi hàm gắn với 1 STATE và ta sẽ sử dụng 1 clock . Thì với clock = 0 thì task A chạy với clock = 2 thì task B chạy, với clock = 5 thì task C chạy, với task C khi clock = 9 thì reset lại clock = 0 để chạy lại từ task A.

Oke nói tóm lại các task chạy lần lượt :)) nhưng vì chạy rất nhanh nên cảm tưởng //.

Oke điểm yếu của code này là task A sẽ chạy đi chạy lại liên tục trong time CLOCK lên 2. Tức là nếu ta muốn thực hiện Task A 1 lần thoi, thì cái thuật toán này đang không tối ưu -> để tối ưu thêm semaphore.

### main.c
```c

enum states{
    STATE_A = 0,
    STATE_B,
    STATE_C
};

typedef enum states State_Type;

State_Type Current_State = STATE_A;
uint32_t CLOCK = 0;

void state_a_function(void);
void state_b_function(void);
void state_c_function(void);

/*mảng function để gọi function*/
void (*state_table[])(void) = {state_a_function, state_b_function,state_c_function };

int main(){
    while(1){
        /* nó check trạng thái state liên tục và gọi hàm mà state đang nắm giữ liên tục*/
        state_table[Current_State]();
        CLOCK++;
    }
}

void state_machine_init(void){
    Current_State = STATE_A;
    CLOCK = 0;
}

void state_a_function(void){
    if(CLOCK == 2){
        printf("END STATE A, START STATE B\n");
        Current_State = STATE_B;
    }
}
void state_b_function(void){
    if(CLOCK == 5){
        printf("END STATE B, START STATE C\n");
        Current_State = STATE_C;
    }
}
void state_c_function(void){
    if(CLOCK == 9){
        printf("END STATE C, START STATE A\n");
        Current_State = STATE_A;
    }
}

```

Ở bài 3 thì ta sử dụng hàm Systick để đếm :)) thay vì cho clock vào while như kia, và làm 1 ứng dụng đếm time chạy thoi. Ncl không có thuật toán gì mới, có thể xem chill


***4.Coding Implementing a Fixed Scheduler with a Finite State Machine**

Khả năng nó sẽ như tên thoi :)) thì code nó sẽ khá giống phần trên chỉ chỉnh sửa theo ứng dụng và cơ bản cái fixed ở đây chính là cài 
void (*state_table[])(void) = {state_a_function, state_b_function,state_c_function }; này. Tức là ta có thể thêm các state hoặc lặp lại các state ở đây :)) tức là fixed trong mảng này. Còn về thuật toán thay vì CLOCK == 2 cố định thì ta sẽ cho CLOCK % 2 và Current_State++. 

Oke thì ta sẽ code ở bên dưới :)) đầu tiên ta sẽ chú í cái mảng state_table[] thì giờ nó sẽ kiểu cho 2 state a liên tiếp hoặc xen kẽ. Thì việc ta dùng Current_State = STATE_A nó không còn hợp lí như ở code trên nữa. vì cái STATE_A này nó là enum, và nó cố định = 0 (STATE_A = 0), thì cái mảng state_table[] này , cái function của state a nó là 0,1 và 3 -> cái trạng thái trong enum như kia không thể dùng được nữa. Tương tự như STATE_B và C cũm không dùng đươc nữa.

Thì thuật toán mới sẽ như sau thì đầu tiên nó sẽ vào state_table[0] tức là state để chạy hàm state_a_function, và mỗi lần ra khỏi hàm ++ CLOCK (clock là 1ms) -> khi đến 2ms sẽ nhảy vào hàm if trong state_a_function của state_table[0], thì trong if nó sẽ tăng Current_State++. Vậy nên lúc này khi ra khỏi hàm state_table sẽ = state_table[1] và nhảy vào state_a_function() của state_table[1] chứ không phải state_a_function() của state_table[0] nữa. Đó tiếp thì khi clock lên 4 nó sẽ nhảy vào if của state_a_function() của state_table[1] và Current_State++. Và lúc này khi ra khỏi hàm  state_table lên 2 hay  state_table[2] trỏ tới hàm state_b_function() và chạy state_b_function(), và lúc này khác với ứng dụng trước ở chỗ là CLOCK nó đang lên 4 ròi, mà if lại = 5 -> state_b_function() chỉ chạy được 1 CLOCK thay vì 3 CLOCK như ở ví dụ bài 2-3 kia. Tại cơ bản 2 cái hàm state_a đã chiếm 4 CLOCK ròi, còn bài bên trên state_a chỉ chiếm có 2 clock. Xong đó tương tự ncl clock cứ %2 là thoát khỏi state_a_func, clock %5 là thoát khỏi state_b_func, clock %9 là thoát khỏi state_c_func

### main.c
```c

enum states{
    STATE_A = 0,
    STATE_B,
    STATE_C
};

typedef enum states State_Type;

State_Type Current_State = STATE_A;
uint32_t CLOCK = 0;

void state_a_function(void);
void state_b_function(void);
void state_c_function(void);

/*mảng function để gọi function*/
void (*state_table[])(void) = {state_a_function, state_a_function, state_b_function,state_a_function,state_c_function,state_b_function };

int main(){
    while(1){
        /* nó check trạng thái state liên tục và gọi hàm mà state đang nắm giữ liên tục*/
        state_table[Current_State]();
        CLOCK++;
    }
}

void state_machine_init(void){
    Current_State = 0;
    CLOCK = 0;
}

void state_a_function(void){
    if(CLOCK % 2){
        Current_State++;
        printf("END STATE A, START STATE B\n");
    }
    /*do st*/
}
void state_b_function(void){
    if(CLOCK % 5){
        Current_State++;
        printf("END STATE B, START STATE C\n");
    }
    /*do st*/
}
void state_c_function(void){
    if(CLOCK % 9){
        Current_State++;
        CLOCK = 0;
        printf("END STATE C, START STATE A\n");
        
    }

    /*do st*/
}

```

</details>


<details><summary><h1> 10. ARM Design Philosophy and RISC Architecture</h1></summary>

Oke loạt bài này sẽ giới thiệu về cấu trúc của ARM, Bus, Memory các kiểu

***1. The RISC Design Philosophy**

RISC hay Reduced Instruction Set Computer tức là nó sẽ giảm độ phức tạp của instructor để cho CPU chạy nhanh hơn và thiết kế đơn giản hơn. Instruction của RISC thường làm việc rất nhỏ, thực thi trong 1 cycle. Thì các cái instruction của nó giống với các lệnh Assembly trong ARM mà mình hay viết.

Ngược lại CISC (Complex Instruction Set Computer) là có kiểu 1 instruction làm được nhiều việc VD instruction này vừa đọc memory vừa cộng vừa ghi vào memory tức là nó sẽ làm 3 việc trên 1 instruction : ADD [0x1000], EAX. Còn với RISC để thực hiện 1 chức năng như kia thì cần tới 3 instruction LDR ADD STR. Thì RISC sẽ giúp MCU đọc nhanh hơn vì nó đọc dòng nào hiểu dòng đấy, còn với CISC là nó phải đọc 1 dòng mà phải hiểu 3.

Oke trong video có nói 4 yếu tố này giờ ta sẽ đi phân tích: 
- Instructions - Reduced number of instructions: RISC chỉ có một số lượng instruction nhỏ và đơn giản, mỗi instruction làm 1 việc rất cụ thể và thường hoàn thành trong 1 clock cycle, ví dụ thì ta đã nói ở bên trên khi so sánh với CISC.

- Pipeline - instructions are executed in parallel by pinelines: tức là muốn nói nhiều instruction chạy song song ở các stage khác nhau. Oke thì nói thêm stage ở đây là những cái gì? Cơ bản 1 pipeline sẽ có 3 bước 1 Fetch(đẩy instruction vào), bước 2 Decode (mã hóa instruct thành các bit để MCU sẵn sàng cho nó chạy dưới dạng dòng điện), bước 3 Execute (thực thi nó dưới dạng dòng điện). Ngoài ra còn 4 Memory và 5 Writeback - Thì 2 cái này ít phổ biến nên ta không nói sâu, bao h cần đụng tới thì nói. Đó thì tức là pineline có 3 bước chính như kia, và mỗi bước là 1 stage, và mỗi stage chứa 1 instruction. Để hiểu rõ hơn ta lấy ví dụ
VD: Giả sử có 3 instruction
```s
I1
I2
I3
```
Pipeline sẽ chạy như này:
Cycle	            Stage
1	                Fetch I1
2	                Decode I1 + Fetch I2
3	                Execute I1 + Decode I2 + Fetch I3
Đấy thì cơ bản cycle 1 lấy I1 ra, cycle thì ta đẩy I1 sang pipeline tiếp theo là decode, và đồng thời pipeline Fetch sẽ push I2 vào, đấy thì cái // chính là cái đồng thời diễn ra trong pipeline.

- Registers - Large general-purpose register set : thì trong RISC-V có tới 32 register trong core, còn với ARM ta hay dùng chỉ có 12 register là từ R0->R12. Thì việc nhiều register trong core có tác dụng gì? Thì ta đều biết rằng việc thao tác với các thanh ghi trong core chỉ diễn ra trong 1 cycle. Ví dụ ta LDR R0, 1 . Thì tức là R0= 1 luôn mà không cần phải trải qua gì nữa, tức là nó chỉ cần gọi ra đúng 1 lần như vậy thao tác đã thành công luôn. Điều đó dẫn tới càng nhiều register core thì trong 1 cycle có thể chạy tới 32 register cùng lúc, thay vì 12 như ở ARM

- Load-Store - Processor operators on data held in register: tức là chỉ có instruction load và store mới truy cập memory, chắc là ở những cấu trúc khác không có cái này, kiểu sử dụng LDR và STR là tương tác từ memory vào thẳng register core
Flow: Memory → Register → ALU → Register → Memory

***2. The ARM Design Philosophy**
Thì nói về cấu trúc ARM được cấu tạo từ 
- RISC: cái này thì không nói nhớ :)) nó như là ngôn ngữ chính của ARM luôn (đương nhiên trong ARM sẽ có chỉnh sửa nhỏ gì đấy mà ta không biết nhưng cơ bản là sẽ giống nhau về mặt ngôn ngữ)
- High Code Denisty: tức là trong ARM đã tối ưu các code instruction sao cho tốn ít memory.
- Power Efficiency: Còn đây đơn giản là tối ưu về mức năng lượng sử dụng

Oke ngoài ra ta còn biết ARM dùng cả Little lẫn Big Edian.

***3. Embedded Systems with ARM Processors**

Oke bài này cơ bản nói về cấu tạo xung quanh của lõi ARM thì sẽ bao gồm bộ Interrupt Controller và Các bus nối ra ngoại vi. 
Ncl nó cũm chỉ có thế thoi :)) xem video nó sẽ nói thêm về đường đi trông như nào.

***4. ARM Bus Technology and AMBA Bus Protocol**
Oke thì nó nói về bus
Về cơ bản có 2 loại bus và gọi bus này có í nghĩa là vai trò loggic chứ không phải là bus thật :))
- Bus master là bus cho ARM processor core - tức là không phải những cái bus này ở gần core mà nó có nghĩa là thiết bị này điều khiển bus, nó ra người ra lệnh cho bus. Thì người ra lệnh đó sẽ được gọi là bus master (người nắm giữ bus của chương trình) - hay CPU :)) ncl CPU chắc chắn là người điều khiển bus ròi
- Bus slave dành cho các perpherals - thì cái này cũm đơn giản là người bị gọi tới thì chính là người nắm giữ bus slave :)). Thì thực ra là ta bỏ từ bus đi cũm được cho dễ hiểu, chứ thực sự nó không liên quan đến bus luôn á :)), nó đơn giản là người bị gọi thì là bus slave, người gọi thì là master slave.

Ví dụ CPU gọi tới UART để lấy dữ liệu thì lúc này, CPU gọi là bus master, còn UART gọi là bus slave.

Oke tiếp đến two bus Architecture levels
- Physical level: Oke thì đơn giản đây là bus vật lý ở trong MCU, 1 bus sẽ bao gồm Data wires, Address wires, Control wires thì gộp 3 wire này sẽ là 32 bit hoặc tùy trường hợp có thể là 16bit (Data wires,Control wires ) thoi. Và 32 bit này truyền // 
- Protocol level: quy tắc giao tiếp trên bus kiểu Ai được phép dùng bus này. Ví dụ như các ABH Bus ấy thì 1 số peripheral như GPIOA,B,C,D sẽ dùng ABH1, còn ví dụ như TIM2 sẽ dùng ABH2 chẳng hạn

Oke 1 số các bus trong ARM - AMBA - Advanced Microcontoller Bus Architecture

- ASB - ARM System Bus
- APB - ARM Peripheral Bus
- AHB - ARM High Performance Bus

Để hiểu rõ các bus ta có thể xem video hoặc lên chat gpt

</details>

***5. Memory**

Phần này ta có thể xem video vì toàn kiến thức biết ròi hoặc là kiến thức hiển nhiên 

***6. Peripherals**

Không biết nói gì về phần này :))

***7. Von Neumann and Harvard architecture**
Oke này như cái tên thì giới thiệu về 2 architecture, thì cái này ta có thể tra ở chat gpt hoặc xem lại video, khi cần thiết, ncl bao h đi pv thì xem lại.

***8.Cache and Tightly Couple Memory**
Bài này nói về cache ncl cũm khá hay nên xem

***9. Memory Management extensions**
Giới thiệu về các bộ quản lý memory là MPU và MMU, thì bài này cũm có thể xem thêm

***10. Co-processor extensions**
Cái này mới lạ phết :)) là nó sẽ 1 cái Co-processor bộ xử lý phụ làm việc cùng CPU. CPU chính sẽ: gửi instruction cho co-processor, co-processor thực hiện tính toán, trả kết quả về CPU.
Vậy tại sao lại cần co-processor vì một số phép tính rất nặng nếu CPU thường làm điển hình như là bộ floating point (bộ này được tích hợp sẵn hardware trong tùy dòng ARM luôn), và các thứ khác vector math, signal processing, encryption thì lúc này co-processor sẽ giúp CPU làm những việc đơn giản hơn.

Có thể xem video để biết thêm thông tin

<details><summary><h1> 11. The ARM Programmer's Model</h1></summary>

Oke loạt bài này sẽ nói về kiểu các thanh ghi bank, các cấu trúc priveledge và pipeline

NCL bài 10 và bài 11 này sẽ tổng hợp các kiến thức về core, các khái niệm là chính, còn đâu sẽ không về RTOS. NCL RTOS vậy là xong ròi :))
Bao h nếu pv thì cta sẽ xem loạt bài 10, 11 để hiểu rõ hơn cấu trúc của ARM

***1. Data Types**
Oke nói về các thuật ngữ data như Byte = 8it, Half-Word = 16 bít, Word = 32 bits
Cơ bản chỉ thế thoi, muốn nghe thêm xem video

***2. Processor Modes**
Chính là mấy cái Privileged mode và UnPrivileged mode ấy. Ngoài ra ta có thể xem thêm video để biết những thứ gì thuộc Privileged và những gì thuộc UnPrivileged

***3. ARM7TDMI Registers**
Oke nó sẽ giới thiệu về bộ register core của ARM7TDMI, ncl bộ register của ARM7TDMI trông cũm khá phức tạp. Ncl bao h động đến dòng này thì tìm hiểu

***4. ARM7TDMI Vector Table**
Oke rất đơn giản thoi :)) thì nói cực kì cơ bản về vector table. Ta có thể xem để biết thêm

***5. ARM Cortex-M Registers**
Oke giới thiệu về bộ register của  ARM Cortex-M, ncl nên xem cũm khá hay để nhớ lại kiến thức, tại nó nói khá rõ các bộ thanh ghi của core.

***6. ARM Cortex-M Vector Table**
Oke bài giới thiệu về vector table của ARM

***7. ARM Data Flow Model**
:)) bài này khá hay nhớ, nói về việc data di chuyển thế nào trong core, ncl khá hay đấy nên xem.

***8. The Pipeline**
Giới thiệu về Pipeline thoi, ncl cũm nên xem để ôn lại kiến thức hoặc là ở bài 10 phần 1. The RISC Design Philosophy ta cũm nói qua về Pipeline ròi, nma xem video nó sẽ trực quan hơn

***9. ARM Processor Family**
Giới thiệu về các dòng ARM

***10. ARM Cortex-A and Cortex-R**
Oke như cái tên thì giới thiệu về 2 dòng ARM Cortex-A and Cortex-R

***11. ARM Cortex-M**
Nói về dòng ARM Cortex-M


Oke vậy là end :)) thì những bài nào nên xem t cũm ghi rõ ròi, oke đến lúc end thoi hehehehehehehhehehhehe

</details>