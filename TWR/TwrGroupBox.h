#ifndef TWRGROUPBOX_H
#define TWRGROUPBOX_H

#include "TwrWnd.h"

class TwrGroupBox : public TwrWnd {
public:
	TwrGroupBox();
	HWND create();
	HWND create(HWND parent);
};

#endif
