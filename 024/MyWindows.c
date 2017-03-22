/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型  
				《Windows 程序设计（SDK）》视频教程                    
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
		MessageBox(NULL, TEXT("这个程序需要在 Windows NT 才能执行！"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, 
		TEXT("鱼C工作室"), 
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
	HPEN hPen, hOldPen;
	HBRUSH hBlueBrush, hRedBrush, hOldBrush;
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		// 辅助线
		hPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		hOldPen = SelectObject(hdc, hPen);
		MoveToEx(hdc, cxClient / 2, 0, NULL);
		LineTo(hdc, cxClient / 2, cyClient);
		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);
		SelectObject(hdc, hOldPen);

		// 头（直径240）
		hBlueBrush = CreateSolidBrush(RGB(0, 159, 132));
		hOldBrush = SelectObject(hdc, hBlueBrush);
		Ellipse(hdc, cxClient / 2 - 120, cyClient / 2 - 200, cxClient / 2 + 120, cyClient / 2 + 40);
		SelectObject(hdc, hOldBrush);

		// 脸（直径200）
		Ellipse(hdc, cxClient / 2 - 100, cyClient / 2 - 160, cxClient / 2 + 100, cyClient / 2 + 40);

		// 眼睛（长60，宽50）
		Ellipse(hdc, cxClient / 2 - 50, cyClient / 2 - 180, cxClient / 2, cyClient / 2 - 120);
		Ellipse(hdc, cxClient / 2, cyClient / 2 - 180, cxClient / 2 + 50, cyClient / 2 - 120);
		hOldBrush = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Ellipse(hdc, cxClient / 2 - 20, cyClient / 2 - 160, cxClient / 2 - 5, cyClient / 2 - 140);
		Ellipse(hdc, cxClient / 2 + 5, cyClient / 2 - 160, cxClient / 2 + 20, cyClient / 2 - 140);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		Ellipse(hdc, cxClient / 2 - 15, cyClient / 2 - 155, cxClient / 2 - 10, cyClient / 2 - 145);
		Ellipse(hdc, cxClient / 2 + 10, cyClient / 2 - 155, cxClient / 2 + 15, cyClient / 2 - 145);
		SelectObject(hdc, hOldBrush);

		// 鼻子
		hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
		hOldBrush = SelectObject(hdc, hRedBrush);
		Ellipse(hdc, cxClient / 2 - 10, cyClient / 2 - 135, cxClient / 2 + 10, cyClient / 2 - 115);
		MoveToEx(hdc, cxClient / 2, cyClient / 2 - 115, NULL);
		LineTo(hdc, cxClient / 2, cyClient / 2 - 30);
		SelectObject(hdc, hOldBrush);

		// 嘴巴
		Arc(hdc, cxClient / 2 - 70, cyClient / 2 - 120, cxClient / 2 + 70, cyClient / 2 - 30, \
			cxClient / 2 - 60, cyClient / 2 - 50, cxClient / 2 + 60, cyClient / 2 - 50);

		// 胡子
		MoveToEx(hdc, cxClient / 2 - 70, cyClient / 2 - 115, NULL);
		LineTo(hdc, cxClient / 2 - 20, cyClient / 2 - 100);
		MoveToEx(hdc, cxClient / 2 - 80, cyClient / 2 - 85, NULL);
		LineTo(hdc, cxClient / 2 - 20, cyClient / 2 - 85);
		MoveToEx(hdc, cxClient / 2 - 70, cyClient / 2 - 55, NULL);
		LineTo(hdc, cxClient / 2 - 20, cyClient / 2 - 70);

		MoveToEx(hdc, cxClient / 2 + 70, cyClient / 2 - 115, NULL);
		LineTo(hdc, cxClient / 2 + 20, cyClient / 2 - 100);
		MoveToEx(hdc, cxClient / 2 + 80, cyClient / 2 - 85, NULL);
		LineTo(hdc, cxClient / 2 + 20, cyClient / 2 - 85);
		MoveToEx(hdc, cxClient / 2 + 70, cyClient / 2 - 55, NULL);
		LineTo(hdc, cxClient / 2 + 20, cyClient / 2 - 70);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}