/*
noMeiryoUI (C) 2005,2012-2018 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef INIREADER_H
#define INIREADER_H

#include <tchar.h>

DWORD GetPrivateProfileStringExT(
	char *sectionName,
	TCHAR *keyName,
	TCHAR *defaultValue,
	TCHAR *returnString,
	DWORD returnSize,
	TCHAR *iniFileName,
	UINT codePage);

#endif
