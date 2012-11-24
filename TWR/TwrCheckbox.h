#ifndef TWRCHECKBOX_H
#define TWRCHECHBOX_H

#include "TwrWnd.h"

class TwrCheckbox : public TwrWnd {
public:
	TwrCheckbox();
	HWND create(void);
	HWND create(HWND parent);
	bool isChecked(void);
	void setChecked(bool checked);
};

#endif
