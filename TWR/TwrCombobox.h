#ifndef TWRCOMBOBOX_H
#define TWRCOMBOBOX_H

#include "TwrWnd.h"

class TwrCombobox : public TwrWnd {
private:
	int style;
public:
	TwrCombobox();
	TwrCombobox(HWND newHwnd);

	HWND create(void);
	HWND create(HWND parent);
	void addItem(TCHAR *item);
	int getSelectedIndex(void);
	void setStyle(int newStyle) {
		style = newStyle;
	}
};

#endif
