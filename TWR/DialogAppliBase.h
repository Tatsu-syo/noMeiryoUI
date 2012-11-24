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
