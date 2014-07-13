/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
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
