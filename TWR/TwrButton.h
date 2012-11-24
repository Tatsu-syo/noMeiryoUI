#ifndef TWRBUTTON_H
#define TWRBUTTON_H

#include "TwrWnd.h"

class TwrButton : public TwrWnd {
public:
	TwrButton();
	HWND create(HMENU id);
	HWND create(HWND parent, HMENU id);
};

#endif
