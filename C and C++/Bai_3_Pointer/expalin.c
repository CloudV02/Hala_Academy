#include <stdio.h>
#include <string.h>

/*Tao struct de luu thong tin sinh vien*/
typedef struct {
   char ten[50];
   float diemTrungBinh;
   int id;
} SinhVien;

int stringCompare(const char *str1, const char *str2) {
   while (*str1 && (*str1 == *str2)) {
       str1++;
       str2++;
   }
   return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}


// Ham so sanh theo ten
int compareByName(const void *a, const void *b) {
   SinhVien *sv1 = (SinhVien *)a; // con tro -- ep kieu con tro SV
   SinhVien *sv2 = (SinhVien *)b; // con tro -- ep kieu con tro SV
   return stringCompare(sv1->ten, sv2->ten);
}

// Ham so sanh theo diem trung binh
int compareByDiemTrungBinh(const void *a, const void *b) {
   SinhVien *sv1 = (SinhVien *)a;
   SinhVien *sv2 = (SinhVien *)b;
   if (sv1->diemTrungBinh > sv2->diemTrungBinh)
   {
       return 1;
   }
  
   return 0;
}

// Ham so sanh theo ID
int compareByID(const void *a, const void *b) {
   SinhVien *sv1 = (SinhVien *)a;
   SinhVien *sv2 = (SinhVien *)b;
   return sv1->id - sv2->id;
}

// Ham sap xep chung
/*Array o day la array ten sinh vien ma tao o trong ham main*/
/*Size kich thuoc cua array*/
/*Function pointer de dien ham minh muon tro toi*/
void sort(SinhVien array[], size_t size, int (*compareFunc)(const void *, const void *)) {
   int i, j;
   SinhVien temp;
   for (i = 0; i < size-1; i++)   
       for (j = i+1; j < size; j++)
           if (compareFunc(array+i, array+j)>0) {
               temp = array[i];
               array[i] = array[j];
               array[j] = temp;
           }
}

void display(SinhVien *array, size_t size) {
   for (size_t i = 0; i < size; i++) {
       printf("ID: %d, Ten: %s, Diem Trung Binh: %.2f\n", array[i].id, array[i].ten, array[i].diemTrungBinh);
   }
   printf("\n");
}

int main() {
   SinhVien danhSachSV[] = {
       {  
           .ten = "Hoang",
           .diemTrungBinh = 7.5,
           .id = 100
       },
       {
           .ten = "Huan",
           .diemTrungBinh = 4.5,
           .id = 101
       },
       {
           .ten = "Vy",
           .diemTrungBinh = 6.8,
           .id = 102},
       {  
           .ten = "Ngan",
           .diemTrungBinh = 5.6,
           .id = 10
       },
   };
   size_t size = sizeof(danhSachSV) / sizeof(danhSachSV[0]);

   // Sap xep theo ten
   sort(danhSachSV, size, compareByName);

   display(danhSachSV, size);

   // Sap xep theo diem trung binh
   sort(danhSachSV, size, compareByDiemTrungBinh);

   display(danhSachSV, size);

   // Sap xep theo ID
   sort(danhSachSV, size, compareByID);

   display(danhSachSV, size);

   return 0;
}





