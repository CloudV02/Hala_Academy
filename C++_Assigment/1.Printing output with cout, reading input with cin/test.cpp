#include <iostream> // thư viện của cin cout tương tác với bên ngoài terminal
#include <string>

using namespace std;


int main(){
    string name;
    int age;
    cout <<"Input name: ";cin>>name;
    cout <<"Input age: ";cin>>age;

    cout<<"Hello, "<<name<<"! You are "<<age<< " years old.";
	
	/*khác thì ta có thể nhập liên tiếp bằng cách này, và dùng space ròi điền value là được*/
	
	cin >> age >> height >> firstNameLetter >> studentBool;
}

