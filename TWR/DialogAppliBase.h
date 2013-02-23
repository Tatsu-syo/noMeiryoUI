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
#pragma once

#include <windows.h>
#include "BaseDialog.h"
#include "../resource.h"

extern HINSTANCE hInst;								// 現在のインターフェイス

void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);

int doEvents();

class DialogAppliBase : public BaseDialog {
protected:
	virtual INT_PTR OnInitDialog();
public:
	virtual ~DialogAppliBase();
	virtual BaseDialog *createBaseDialog();
	virtual int OnAppliStart(TCHAR *lpCmdLine);
	virtual int OnWindowShow();
	virtual int OnAppliEnd();

};

extern DialogAppliBase *createAppli();
extern INT_PTR CALLBACK MainDialogProc(
  HWND hwndDlg,  // ダイアログボックスのハンドル
  UINT uMsg,     // メッセージ
  WPARAM wParam, // 最初のメッセージパラメータ
  LPARAM lParam  // 2 番目のメッセージパラメータ
);
