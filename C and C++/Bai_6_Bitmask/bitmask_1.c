#include <stdio.h>
#include <stdint.h>

#define GENDER      1 << 0 
#define TSHIRT      1 << 1
#define HAT         1 << 2
#define SHOES       1 << 3

// Cac tinh nang khac
#define FEATURE1    1 << 4
#define FEATURE2    1 << 5
#define FEATURE3    1 << 6
#define FEATURE4    1 << 7

void enableFeature(uint8_t *option, uint8_t feature){
    *option |= feature;
}
void disableFeature(uint8_t *option, uint8_t feature){
    *option &= ~(feature);
}
int isFeatureEnabled(uint8_t option, uint8_t feature){
    return (option & feature);
}
void listSelectedFeatures(uint8_t option){
    printf("Selected feature:\n");
    if(isFeatureEnabled(option,GENDER)){
        printf("- Gender\n");
    }
    if(isFeatureEnabled(option,TSHIRT)){
        printf("- T-Shirt\n");
    }
    if(isFeatureEnabled(option,HAT)){
        printf("- Hat\n");
    }
    if(isFeatureEnabled(option,SHOES)){
        printf("- Shoes\n");
    }
    for (int i = 0; i < 8; i++)
    {
        printf("feature selected: %d\n",(option>>i)&1);
    }
    

}

int main(){
    uint8_t options = 0;
    
    // them tinh nang
    enableFeature(&options,GENDER|TSHIRT|HAT|SHOES );

    // bo tinh nang
    disableFeature(&options,TSHIRT);

    // Liet ke cac tinh nang da chon
    listSelectedFeatures(options);

    return 0;

}