#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	BOOLEAN flag = TRUE;
	int seeresult = -1;
	int retryresult = -1;

	while (flag) {
		seeresult = MessageBox(NULL, TEXT("�㿴����Ϣ����"), TEXT("��ʾ"), MB_YESNO);
		if (IDYES == seeresult) {
			MessageBox(NULL, TEXT("�ţ������ͺ�"), TEXT("��ʾ"), MB_OK);
			retryresult = MessageBox(NULL, TEXT("������"), TEXT("��ʾ"), MB_RETRYCANCEL);
			if (IDRETRY == retryresult) {
				continue;
			} else {
				break;
			}
		}
		else {
			MessageBox(NULL, TEXT("�������۾���ϸ���ƣ��Ҿ�����Ϣ�򰡣�"), TEXT("��ʾ"), MB_OK);
			continue;
		}
	}

	return 0;
}