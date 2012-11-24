#pragma once

#include <windows.h>
#include "TwrCore.h"
#include "BaseForm.h"
#include "../resource.h"

// アプリケーション共通定義

void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);

int doEvents();

class WindowAppliBase : public BaseForm {
public:
	virtual ~WindowAppliBase();
	virtual BaseForm *createBaseForm();
	virtual int OnAppliStart(TCHAR *lpCmdLine);
	virtual int OnWindowShow();
	virtual int OnAppliEnd();

};

// ウインドウアプリ共通定義

#define MAX_LOADSTRING 100

extern TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

WindowAppliBase *createAppli();
ATOM	MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
