#include "Widget.h"
#include <iostream>
using namespace std;

Widget::Widget()
{
	cout << "Widget created" << endl;
}

Widget::~Widget()
{
	cout << "Widget destroyed" << endl;
}

void Widget::use()
{
	cout << "Using widget" << endl;
}
