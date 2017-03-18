/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>
#include <StrSafe.h>

#define NUM		1000
#define TWOPI	(2 * 3.14159)
#define PI (3.14159)

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
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static int cxClient, cyClient;
	POINT apt1[NUM];
	POINT apt2[NUM];
	int i, j;
	size_t iTarget = 0;
	int x = 0, y = 0;
	TCHAR szBuffer[64];
	TCHAR szColBuffer[7][7] = { TEXT("3/4"), TEXT("1/2"), TEXT("1/4"), TEXT("0"), TEXT("-1/4"), TEXT("-1/2"), TEXT("-3/4") };
	TCHAR szRowBuffer[7][7] = { TEXT("-3/4��"), TEXT("-1/2��"), TEXT("-1/4��"), TEXT("0"), TEXT("1/4��"), TEXT("1/2��"), TEXT("3/4��") };

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		// ���Ƹ�����
		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);
		MoveToEx(hdc, cxClient / 2, 0, NULL);
		LineTo(hdc, cxClient / 2, cyClient);
		for (i = 0; i < 8; ++i) {

			x += cxClient / 8;
			MoveToEx(hdc, x, 0, NULL);
			LineTo(hdc, x, cyClient);

			y += cyClient / 8;
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, cxClient, y);

			// ��ӡ������
			if (i != 7) {
				StringCchLength(szColBuffer[i], 7, &iTarget);
				TextOut(hdc, cxClient / 2 + 10, y + 10, szColBuffer[i], iTarget);
				StringCchLength(szRowBuffer[i], 7, &iTarget);
				TextOut(hdc, x + 10, cyClient / 2 + 10, szRowBuffer[i], iTarget);
			}
		}
		// �������Һ���
		for (i = 0; i < NUM; ++i) {
			apt1[i].x = i * cxClient / NUM;
			apt1[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / NUM + PI)));
		}
		Polyline(hdc, apt1, NUM);
		// ������һ������
		for (j = 0; j < NUM; ++j) {
			apt2[j].x = j * cxClient / NUM;
			apt2[j].y = (int)(cyClient / 2 * (1 - sin(TWOPI * j / NUM + PI * 3/ 2)));
		}
		Polyline(hdc, apt2, NUM);
		// �������Ͻǵ�������
		StringCchCopy(szBuffer, 64, TEXT("sin"));
		StringCchLength(szBuffer, 64, &iTarget);
		TextOut(hdc, cxClient - 90, 10, szBuffer, iTarget);
		MoveToEx(hdc, cxClient - 60, 20, NULL);
		LineTo(hdc, cxClient - 40, 20);
		StringCchCopy(szBuffer, 64, TEXT("cos"));
		StringCchLength(szBuffer, 64, &iTarget);
		TextOut(hdc, cxClient - 90, 25, szBuffer, iTarget);
		MoveToEx(hdc, cxClient - 60, 35, NULL);
		LineTo(hdc, cxClient - 40, 35);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}