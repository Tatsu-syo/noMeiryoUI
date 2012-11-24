#ifndef TWRTAB_H
#define TWRTAB_H

#include "TwrWnd.h"

class TwrTab : public TwrWnd {
public:
	TwrTab();
	HWND create();
	HWND create(HWND parent);
	void add(TCHAR *title);
	int getSelectedIndex(void);
	void select(int index);
};

#endif
