#include	<stdio.h>
#include	<locale.h>

int main(int argc,char *argv[])
{
	printf("%s\n",setlocale( LC_ALL, "" ));

	return 0;
}
