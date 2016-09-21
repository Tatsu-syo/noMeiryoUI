/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#pragma once

#include <windows.h>
#include "../resource.h"

extern HINSTANCE hInst;								// 現在のインターフェイス

void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);

int doEvents();
