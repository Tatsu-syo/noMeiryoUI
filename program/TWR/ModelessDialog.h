/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef MODELESSDIALOG_H
#define MODELESSDIALOG_H

#include <windows.h>

void initModelessDialog(void);
void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);
bool handleDialogMessage(LPMSG lpMsg);

#endif
