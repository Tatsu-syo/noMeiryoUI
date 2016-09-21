/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef TWRCHECKBOX_H
#define TWRCHECHBOX_H

#include "TwrWnd.h"

class TwrCheckbox : public TwrWnd {
public:
	TwrCheckbox();
	TwrCheckbox(HWND newHwnd);
	HWND create(void);
	HWND create(HWND parent);
	bool isChecked(void);
	void setChecked(bool checked);
};

#endif
