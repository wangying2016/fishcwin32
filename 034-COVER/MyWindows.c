/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <strsafe.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MyWindows");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, 
		TEXT("��C������"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		NULL, 
		NULL, 
		hInstance, 
		NULL);
	
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
	static int cLinesMax, cLines;
	static PMSG pmsg;
	static RECT rectScroll;
	static TCHAR szTop[] = TEXT("Message        Key       Char     Repeat Scan Ext ALT Prev Tran");
	static TCHAR szUnd[] = TEXT("_______        ___       ____     ______ ____ ___ ___ ____ ____");
	static TCHAR *szFormat[2] = {
		TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
		TEXT("%-13s            0x%04X%1s%c %6u %4d %3s %3s %4s %4s")
	};
	static TCHAR *szYes = TEXT("Yes");
	static TCHAR *szNo = TEXT("No");
	static TCHAR *szDown = TEXT("Down");
	static TCHAR *szUp = TEXT("Up");
	static TCHAR *szMessage[] = {
		TEXT("WM_KEYDOWN"), TEXT("WM_KEYUP"), TEXT("WM_CHAR"), TEXT("WM_DEADCHAR"),
		TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"), TEXT("WM_SYSCHAR"), TEXT("WM_SYSDEADCHAR")
	};
	HDC hdc;
	int i, iType;
	int iTarget;
	PAINTSTRUCT ps;
	TCHAR szBuffer[128], szKeyName[32];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
	case WM_DISPLAYCHANGE:
		// ��ȡ�ͻ��������ߴ�
		cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);

		hdc = GetDC(hwnd);

		// ���õȿ�����
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;

		ReleaseDC(hwnd, hdc);

		if (pmsg) {
			free(pmsg);
		}

		// ����ͻ���һ�����������ɶ�����
		cLinesMax = cyClientMax / cyChar;
		// �����㹻���� cLinesMax ����Ϣ���ڴ�
		pmsg = malloc(cLinesMax * sizeof(MSG));
		cLines = 0;

		// ����û�� break��û�� return 

	case WM_SIZE:
		if (message == WM_SIZE) {
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}

		// ��������ľ���
		rectScroll.left = 0;
		rectScroll.right = cxClient;
		rectScroll.top = cyChar;
		rectScroll.bottom = cyChar * (cyClient / cyChar);

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		// �������յ�������Ϣ��ҳ�����Ϲ���һ��
		for (i = cLinesMax - 1; i > 0; i--) {
			pmsg[i] = pmsg[i - 1];
		}
		pmsg[0].hwnd = hwnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;

		cLines = min(cLines + 1, cLinesMax);
		ScrollWindow(hwnd, 0, -cyChar, &rectScroll, &rectScroll);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc, TRANSPARENT);
		StringCchLength(szTop, sizeof(szTop), &iTarget);
		TextOut(hdc, 0, 0, szTop, iTarget);
		StringCchLength(szUnd, sizeof(szUnd), &iTarget);
		TextOut(hdc, 0, 0, szUnd, iTarget);

		for (i = 0; i < min(cLines, cyClient / cyChar - 1); i++) {
			iType = pmsg[i].message == WM_CHAR ||
				pmsg[i].message == WM_SYSCHAR ||
				pmsg[i].message == WM_DEADCHAR ||
				pmsg[i].message == WM_SYSDEADCHAR;

			GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));
			TextOut(hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer,
				wsprintf(szBuffer, szFormat[iType],
					szMessage[pmsg[i].message - WM_KEYFIRST],
					pmsg[i].wParam,
					(PTSTR)(iType ? TEXT(" ") : szKeyName),
					(TCHAR)(iType ? pmsg[i].wParam : ' '),
					LOWORD(pmsg[i].lParam), HIWORD(pmsg[i].lParam) & 0xFF,
					0x01000000 & pmsg[i].lParam ? szYes : szNo,
					0x20000000 & pmsg[i].lParam ? szYes : szNo,
					0x40000000 & pmsg[i].lParam ? szDown : szUp,
					0x80000000 & pmsg[i].lParam ? szUp : szDown));
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}