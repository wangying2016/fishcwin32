#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	BOOLEAN flag = TRUE;
	int seeresult = -1;
	int retryresult = -1;

	while (flag) {
		seeresult = MessageBox(NULL, TEXT("你看见消息框吗？"), TEXT("提示"), MB_YESNO);
		if (IDYES == seeresult) {
			MessageBox(NULL, TEXT("嗯，看见就好"), TEXT("提示"), MB_OK);
			retryresult = MessageBox(NULL, TEXT("重试吗？"), TEXT("提示"), MB_RETRYCANCEL);
			if (IDRETRY == retryresult) {
				continue;
			} else {
				break;
			}
		}
		else {
			MessageBox(NULL, TEXT("你睁大眼睛仔细瞧瞧，我就是消息框啊！"), TEXT("提示"), MB_OK);
			continue;
		}
	}

	return 0;
}