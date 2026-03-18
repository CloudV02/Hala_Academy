#ifndef DISPLAY_H
#define DISPLAY_H

#include "Observer.h"

class Display : public Observer
{
	public:
		void update(float temperature) override;
};

#endif
