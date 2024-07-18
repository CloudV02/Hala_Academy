1. Tạo project KeliC

2. Viết chương trình liên quan đến thanh ghi
- Mọi hoạt động trong MCU đều cần xung Clock -> GPIO hoạt động thì ta cần cấp clock cho ngoại vi GPIO

- GPIOC sử dụng đường bus APB2 -> cấu hình cho đường bus APB2 cấp xung cho Port C

- Vì Port C or GPIOC có nhiều chân -> phải cấu hình cho chân được sử dụng

- GPIO có 2 chế độ vào ra chính, với mỗi chế độ sẽ cấu hình khác nhau tùy thuộc vào chương trình
+ Với input mode thì có:
. Analog mode: Tín hiệu vào là analog
. Floating input: Thả nổi ở mức giữa 0V và 3.3V (không chắc)
. Input with pull-up/ pull-down: Đầu vào kéo lên hoặc kéo xuống tương đương với 3.3v hoặc 0v
+ Với output mode thì có:
. General purpose output push-pull: Tín hiệu ra là digital 3.3v hoặc 0v
. 