/*
This file is part of noMeiryoUI
Copyright (C) 2005,2012,2013 Tatsuhiko Shoji

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
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
