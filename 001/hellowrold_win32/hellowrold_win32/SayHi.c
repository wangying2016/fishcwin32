#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	BOOLEAN flag = TRUE;
	int result;
	while (flag) {
		result = MessageBox(NULL, TEXT("��������İ��ҹر��𣿣�"), TEXT("��C������"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (IDYES == result) {
			flag = FALSE;
		}
	}
	
	return 0;
}