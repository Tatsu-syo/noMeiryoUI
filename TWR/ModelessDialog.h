#ifndef MODELESSDIALOG_H
#define MODELESSDIALOG_H

#include <windows.h>

void initModelessDialog(void);
void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);
bool handleDialogMessage(LPMSG lpMsg);

#endif
