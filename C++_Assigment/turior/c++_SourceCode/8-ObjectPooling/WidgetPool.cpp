#include "WidgetPool.h"
#include "Widget.h"

WidgetPool::~WidgetPool()
{
	for(Widget* w : allWidgets)
	{
		delete w;
	}
}

Widget* WidgetPool::acquire()
{
	if (pool.empty())
	{
		Widget* newWidget = new Widget(); // create a new object if pool is empty
		allWidgets.push_back(newWidget); // track widgets to delete them (created with new)
		return newWidget;
	}
	else
	{
		Widget* widget = pool.front();
		pool.pop();
		return widget;
	}
}

void WidgetPool::release(Widget* widget)
{
	pool.push(widget); // return object to the pool
}

