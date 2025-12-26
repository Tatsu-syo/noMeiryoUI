/*
noMeiryoUI (C) 2005,2012-2021 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef INIREADER_H
#define INIREADER_H

#include <tchar.h>

DWORD GetPrivateProfileStringExT(
	char *sectionName,
	TCHAR *keyName,
	TCHAR *defaultValue,
	tstring &returnString,
	TCHAR *iniFileName);

#endif
