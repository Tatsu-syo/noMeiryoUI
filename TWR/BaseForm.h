#ifndef BASEFORM_H
#define BASEFORM_H

#include "TwrWnd.h"
#include "BaseDialog.h"

/**
 * T windows api WRapper<br>
 * Window base class
 */
class BaseForm : public TwrWnd {
private:
	TCHAR *windowClass;
	TCHAR *title;
protected:
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	HINSTANCE hInstance;
	DWORD style;
public:
	BaseForm();
	virtual ~BaseForm();
	void setClass(TCHAR *windowClassName) {
		windowClass = windowClassName;
	}
	void setTitle(TCHAR *newTitle) {
		title = newTitle;
	}
	HWND add(TwrWnd *obj);
	HWND add(BaseDialog *obj);
	void setStyle(DWORD newStyle);
	virtual void UpdateData(bool toObj);

	virtual HWND Create(HINSTANCE hInstance);
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCommand(int wmId, int wmEvent);
	virtual LRESULT OnCreate();
	virtual LRESULT OnNotify(LPNMHDR nmhdr);
	virtual LRESULT OnDestroy();


};

// MFC‚ÌDDX‚à‚Ç‚«ƒ}ƒNƒ
#define FDX_Text(toObj, ctl, str, bufLen) \
	if (toObj) { \
		SendMessage(ctl->getHwnd(),WM_GETTEXT,bufLen,str); \
	} else { \
		SendMessage(ctl->getHwnd(),WM_SETTEXT,(WPARAM)0,str); \
	}

#define FDX_Check(toObj, ctl, stat) \
	if (toObj) { \
		stat = (SendMessage(ctl->getHwnd(),BM_GETCHECK,0,0) ? true : false); \
	} else { \
		SendMessage(ctl->getHwnd(),BM_SETCHECK,(WPARAM)(stat ? 1 : 0),0); \
	}

#define FDX_CBIndex(toObj, ctl, index) \
	if (toObj) { \
		index = SendMessage(ctl->getHwnd(),CB_GETCURSEL,0,0); \
	} else { \
		SendMessage(ctl->getHwnd(),CB_SETCURSEL,(WPARAM)index,0); \
	}


#endif // BASEFORM_H
