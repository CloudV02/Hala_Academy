#include <stdio.h>
#define PRINT_MENU(...) \
const char *items[] = {__VA_ARGS__}; \
int size = sizeof(items) / sizeof(items[0]); \
int i; \
for(i = 0; i<size ; i++){ \
    printf("%d. %s\n", i+1, items[i]); \
}
#define CASE_OPTION(number, function) case number: function(); break;
#define HANDLE_OPTION(option, ...) \
switch (option) \
    { \
        __VA_ARGS__ \
        default: printf("Invalid option"); break; \
    }

void feature1() { printf("Feature 1 selected\n"); }
void feature2() { printf("Feature 2 selected\n"); }
void feature3() { printf("Feature 3 selected\n"); }
void feature4() { printf("Feature 4 selected\n"); }


int main()
{
    // printf("1. Option 1\n");
    // printf("2. Option 2\n");
    // printf("3. Option 3\n");
    // printf("4. Option 4\n");
    // printf("5. Exit\n");
    PRINT_MENU("Option 1","Option 2")

    // Giả sử option được nhập từ người dùng
    int option ;
    scanf("%d", &option);

    HANDLE_OPTION(option, 
    CASE_OPTION(1,feature1)
    CASE_OPTION(2,feature2)
    CASE_OPTION(3,feature3)
    CASE_OPTION(4,feature4));


    return 0;
}


