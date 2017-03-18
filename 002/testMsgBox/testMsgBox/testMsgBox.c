#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MessageBox(NULL, TEXT("ÄãºÃÑ½"), TEXT("´òÕÐºô"), MB_YESNO | MB_OKCANCEL);

	return 0;
}