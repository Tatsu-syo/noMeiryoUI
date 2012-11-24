#ifndef TWRLISTVIEW_H
#define TWRLISTVIEW_H

#include "commctrl.h"

#include "TwrWnd.h"

class TwrListView : public TwrWnd {
public:
	TwrListView();
	TwrListView(HWND newHwnd);

	HWND create(void);
	void insertItem(int index, TCHAR *item);
	unsigned int SetExtendedStyle(unsigned int newStyle);
	int insertColumn(
		int no,
		TCHAR *title, 
		int nFormat = LVCFMT_LEFT,
		int nWidth = -1,
		int nSubItem = -1 
	);
	void checkItem(int index, bool checked);
	void selectItem(int index);
	bool isChecked(int index);
};

#endif
