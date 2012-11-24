#ifndef TWRTEXT_H
#define TWRTEXT_H

#include "TwrWnd.h"

class TwrText : public TwrWnd {
public:
	TwrText();
	HWND create();
	HWND create(HWND parent);
};

#endif
