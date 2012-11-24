#ifndef TWRMENU_H
#define TWRMENU_H

class TwrMenu {
private:
	HMENU menuHandle;
public:
	TwrMenu(HWND wnd);
	void CheckMenuItem(int id,bool checked);
	bool isChecked(int item);
};


#endif
