#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include "TwrWnd.h"
#include "TwrCombobox.h"

class BaseDialog : public TwrWnd {
private:
	HWND parentWindow;
	int dialogResource;
	bool isModeless;
protected:
	static BaseDialog *modalCallback;
	virtual INT_PTR OnInitDialog();
	virtual INT_PTR OnCommand(WPARAM wParam);
	TCHAR ddx_buf[512];
public:
	BaseDialog();
	BaseDialog(HWND parent, int resource);
	virtual ~BaseDialog();

	INT_PTR showModal();
	HWND showModeless();
	static INT_PTR CALLBACK modalDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK modelessDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK dialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void setParent(HWND parent)
	{
		parentWindow = parent;
	}

	TwrWnd *GetDlgItem(int item);
	HWND GetDlgItemRaw(int item);

	virtual void UpdateData(bool toObj);

};

// MFCのDDXもどきForm Data eXchangeマクロ
// UpdateData関数の引数、ダイアログコントロールID、設定対象の変数

// コントロールのテキスト
#define DDX_Text(toObj, ctl, str) \
	if (toObj) { \
		GetDlgItemText(hWnd,ctl,ddx_buf,512); \
		str = ddx_buf; \
	} else { \
		SetDlgItemText(hWnd,ctl,str.c_str()); \
	}

// チェックボックスのチェック状態
#define DDX_Check(toObj, ctl, stat) \
	if (toObj) { \
		stat = SendMessage(::GetDlgItem(hWnd,ctl),BM_GETCHECK,0,0); \
	} else { \
		SendMessage(::GetDlgItem(hWnd,ctl),BM_SETCHECK,(WPARAM)stat,0); \
	}

// コンボボックスのインデックス
#define DDX_CBIndex(toObj, ctl, index) \
	if (toObj) { \
		index = SendMessage(::GetDlgItem(hWnd,ctl),CB_GETCURSEL,0,0); \
	} else { \
		SendMessage(::GetDlgItem(hWnd,ctl),CB_SETCURSEL,(WPARAM)index,0); \
	}


#endif
