#include <stdio.h>
#include <stdint.h>


#define GENDER        1 << 0  // Bit 0: Giới tính (0 = Nữ, 1 = Nam)
#define TSHIRT        1 << 1  // Bit 1: Áo thun (0 = Không, 1 = Có)
#define HAT           1 << 2  // Bit 2: Nón (0 = Không, 1 = Có)
#define SHOES         1 << 3  // Bit 3: Giày (0 = Không, 1 = Có)
// Tự thêm tính năng khác
#define FEATURE1      1 << 4  // Bit 4: Tính năng 1
#define FEATURE2      1 << 5  // Bit 5: Tính năng 2
#define FEATURE3      1 << 6  // Bit 6: Tính năng 3
#define FEATURE4      1 << 7  // Bit 7: Tính năng 4

void enableFeature(uint8_t *option, uint8_t feature) {
    *option |= feature; // feature 0x00000111
}

void disableFeature(uint8_t *option, uint8_t feature) {
    *option &= ~feature; // *option = 0x00000111 & (0x11111101) -> 0x00000101
}


int isFeatureEnabled(uint8_t option, uint8_t feature) {
    return (option & feature) != 0;
}

void listSelectedFeatures(uint8_t option) {
    printf("Selected Features:\n");

    if (option & GENDER) { // 0x00000101 & 0x000000001
        printf("- Gender\n");
    }
    if (option & TSHIRT) { 
        printf("- T-Shirt\n");
    }
    if (option & HAT) {
        printf("- Hat\n");
    }
    if (option & SHOES) {
        printf("- Shoes\n");
    }

    for (int i = 0; i < 8; i++)
    {
        printf("feature selected: %d\n", (option >> i) & 1); // 0x00000001 & 0x00000001
    }
    

    // Thêm các điều kiện kiểm tra cho các tính năng khác
}



int main() {
    uint8_t options = 0; // 0x00000101

    // Thêm tính năng 
    enableFeature(&options, GENDER | TSHIRT | HAT);

    disableFeature(&options, TSHIRT);

    // Liệt kê các tính năng đã chọn
    listSelectedFeatures(options);
    
    return 0;
}



