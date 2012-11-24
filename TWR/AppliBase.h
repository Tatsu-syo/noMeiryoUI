#pragma once

#include <windows.h>
#include "../resource.h"

extern HINSTANCE hInst;								// 現在のインターフェイス

void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);

int doEvents();
