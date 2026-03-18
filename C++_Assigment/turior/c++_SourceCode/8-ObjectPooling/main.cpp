#include "WidgetPool.h"

int main()
{
	WidgetPool pool;
	Widget* widget1 = pool.acquire();
	widget1->use();
	pool.release(widget1);

	Widget* widget2 = pool.acquire();
	widget2->use();
	pool.release(widget2);

	return 0;
}
