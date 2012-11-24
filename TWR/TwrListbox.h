#ifndef TWRLISTBOX_H
#define TWRLISTBOX_H

#include "TwrWnd.h"

class TwrListbox : public TwrWnd {
public:
	TwrListbox();
	TwrListbox(HWND newHwnd);

	HWND create(void);
	void addItem(TCHAR *item);
	void insertItem(int index, TCHAR *item);
	void removeItem(int index);
	void selectItem(int index);
	int getSelectedIndex(void);
};

#endif
