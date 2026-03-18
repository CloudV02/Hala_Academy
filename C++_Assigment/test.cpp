//WRITE INCLUDE/USING BELOW THIS LINE
#include <iostream>
#include <string>
//WRITE INCLUDE/USING UPPER THAN THIS LINE


void displayHumanInfo();

using namespace std;

int main(){
    displayHumanInfo();
    return 0;
}

void displayHumanInfo()
{
    //WRITE CODE BELOW THIS LINE
    
    unsigned int age;
    double height;
    char firstNameLetter;
    bool studentBool = true;
    
    cin >> age >> height >> firstNameLetter >> studentBool;
    
    cout << age << endl;
    cout << height << endl;
    cout << firstNameLetter << endl;
    cout << studentBool << endl;
    
    cout << "Your age in 5 years "<< age+5 <<endl;
    cout << "Your height in centimeters "<< height*100 << endl;
    
    cout << age << endl;
    cout << height << endl;
    cout << firstNameLetter << endl;
    cout << studentBool << endl;

    //WRITE CODE UPPER THAN THIS LINE
}