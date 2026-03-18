#ifndef WIDGETPOOL_H
#define WIDGETPOOL_H

#include <queue>
#include "Widget.h"
using namespace std;

class WidgetPool
{
	private:
		queue<Widget*> pool;
		vector<Widget*> allWidgets;
	public:
		~WidgetPool();
		Widget* acquire();
		void release(Widget* widget);
};

#endif
