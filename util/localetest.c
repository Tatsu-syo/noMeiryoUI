/*
noMeiryoUI (C) 2005,2012-2017 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include	<stdio.h>
#include	<locale.h>

int main(int argc,char *argv[])
{
	printf("%s\n",setlocale( LC_ALL, "" ));

	return 0;
}
