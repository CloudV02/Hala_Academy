//WRITE INCLUDE/USING BELOW THIS LINE
#include <iostream>
using namespace std;
//WRITE INCLUDE/USING UPPER THAN THIS LINE

int main(){
	int a, b;
	bool b1, b2;
	cin >> a >> b >> b1 >> b2;
	calculationAndOperations(a,b,b1,b2);
	return 0;
}

void calculationAndOperations(int a, int b, bool b1, bool b2)
{
    //WRITE CODE BELOW THIS LINE
    
    cout << "Sum: "<< a << " + " << b << " = "<< a+b << endl;
    cout << "Difference: "<< a << " - " << b << " = "<< a-b << endl;
    cout << "Product: "<< a << " * " << b << " = "<< a*b << endl;
    cout << "Quotient: "<< a << " / " << b << " = "<< static_cast<double>(a)/b << endl; 
	// ngoài ra còn cast kiểu (double)(a) / (double)(b), nhưng mà cast kiểu này được cho là kiểu cast của C, chứ không phải của C++
	
    cout << "Modulus: "<< a << " % " << b << " = "<< a%b << endl;
    cout << boolalpha; // khi ta gọi cái này thì các boolen thay vì in ra là 1 thì sẽ in ra là chữ true
    cout << "AND Operation: "<< b1 << " && " << b2 << " = " << (b1&&b2) << endl;
    cout << "OR Operation: "<< b1 << " || " << b2 << " = " << (b1||b2) << endl;
    cout << "NOT Operation: "<<"!"<< b1 << " = " << (!b1) << endl;

    //WRITE CODE UPPER THAN THIS LINE 
}