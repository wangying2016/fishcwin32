#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main() 
{
	wchar_t c = L'��';

	setlocale(LC_ALL, "Chs");
	wprintf(L"%lc\n", c);

	return 0;
}