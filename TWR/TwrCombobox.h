/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
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
	void addItem(const TCHAR *item);
	int getSelectedIndex(void);
	void setStyle(int newStyle) {
		style = newStyle;
	}
	void clear(void);
	void setSelectedIndex(int index);
	tstring getSelectedText(void);

};

#endif
