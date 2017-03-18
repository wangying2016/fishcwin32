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

HRESULT _DrawStrongPoint(HDC hdc, int x, int y)
{	
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int cxClient, cyClient;
	static POINT apt[4];

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		hdc = GetDC(hwnd);
		// д����㡢�յ��λ��
		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;
		apt[3].x = cxClient * 3 / 4;
		apt[3].y = cyClient / 2;
		// ����ˢ����ʾ
		InvalidateRect(hwnd, NULL, TRUE);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_LBUTTONDOWN:
		apt[1].x = LOWORD(lParam);
		apt[1].y = HIWORD(lParam);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_RBUTTONDOWN:
		apt[2].x = LOWORD(lParam);
		apt[2].y = HIWORD(lParam);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// ���Ʊ���������
		PolyBezier(hdc, apt, 4);
		// �������ص�
		MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
		for (int i = 0; i < 4; ++i) {
			_DrawStrongPoint(hdc, apt[i].x, apt[i].y);
			LineTo(hdc, apt[i].x, apt[i].y);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}