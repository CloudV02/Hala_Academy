#include <iostream>
using namespace std;
#include "Person.h"
#include "LibraryMember.h"
#include "Book.h"
#include "Library.h"

int main() {
    // Demonstrate polymorphism with Person and LibraryMember
    Person* person1 = new Person("Alice");
    Person* member1 = new LibraryMember("Bob", "M123");

    cout << "=== Person Info ===" << endl;
    person1->displayInfo();  // Calls Person::displayInfo

    cout << "\n=== Library Member Info ===" << endl;
    member1->displayInfo();  // Calls LibraryMember::displayInfo (overridden)

    // Clean up
    delete person1;
    delete member1;

    // Demonstrate Library and Book
    Library library;
    Book book1("1984", "George Orwell");
    Book book2("To Kill a Mockingbird", "Harper Lee");

    library.addBook(book1);
    library.addBook(book2);

    cout << "\n=== Library Books ===" << endl;
    library.displayBooks();

    return 0;
}
