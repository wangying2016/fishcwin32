#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MessageBox(NULL, TEXT("���ѽ"), TEXT("���к�"), MB_YESNO | MB_OKCANCEL);

	return 0;
}