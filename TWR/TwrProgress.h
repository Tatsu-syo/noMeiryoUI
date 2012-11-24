#ifndef TWRPROGRESS_H
#define TWRPROGRESS_H

#include <commctrl.h>
#include "TwrWnd.h"

class TwrProgress : public TwrWnd {

public:
	TwrProgress();
	TwrProgress(HWND newHwnd);

	void setRange(unsigned int min, unsigned int max);
	void setStep(int step);
	void setPos(int pos);
	void stepIt(void);
};

#endif
