/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
--------------------------------------------------------------------*/

#include <windows.h>

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
		TEXT("��win32�������ư���ͼ"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		550, 
		550,
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
	static int cxClient, cyClient;
	int radius = 0;
	HPEN hPen, hOldPen;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		radius = (cxClient - 100) / 2;
		int small_radius = radius / 6;

		// ��Բ
		Ellipse(hdc, 50, 50, cxClient - 50, cxClient - 50);
		// ��С��Բ�ʹ��Բ
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Chord(hdc, 50 + radius / 2, 50, 50 + radius * 3 / 2, 50 + radius, 50 + radius, 50 + radius, 50 + radius, 50);
		Chord(hdc, 50, 50, cxClient - 50, cxClient - 50, 50 + radius, 50, 50 + radius, 50 + 2 * radius);
		Ellipse(hdc, 50 + radius - small_radius, 50 + radius / 2 - small_radius, 50 + radius + small_radius, 50 + radius / 2 + small_radius);
		// ��С��Բ
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		Chord(hdc, 50 + radius / 2, 50 + radius, 50 + radius * 3 / 2, 50 + 2 * radius, 50 + radius, 50 + radius, 50 + radius, 50 + 2 * radius);
		SetDCPenColor(hdc, RGB(255, 255, 255));

		// ���Ƕ��������
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		hOldPen = SelectObject(hdc, hPen);
		MoveToEx(hdc, 50 + radius, 50 + radius, NULL);
		LineTo(hdc, 50 + radius, 50 + 2 * radius);
		SelectObject(hdc, hOldPen);
		
		Ellipse(hdc, 50 + radius - small_radius, 50 + radius / 2 - small_radius, 50 + radius + small_radius, 50 + radius / 2 + small_radius);
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Ellipse(hdc, 50 + radius - small_radius, 50 + radius * 4 / 3, 50 + radius + small_radius, 50 + radius * 3 / 2 + small_radius);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}