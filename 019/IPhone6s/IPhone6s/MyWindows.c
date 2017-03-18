/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型  
				《Windows 程序设计（SDK）》视频教程                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <StrSafe.h>

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
		TEXT("用win32函数绘制iPhone6s"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		420, 
		620,
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
	static int cxClient, cyClient, cxChar, cyChar;
	TEXTMETRIC tm;
	TCHAR szBuffer[16];
	size_t iTarget = 0;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		// 绘制标题
		StringCchCopy(szBuffer, 16, TEXT("iPhone6"));
		StringCchLength(szBuffer, 16, &iTarget);
		TextOut(hdc, cxClient / 2 - cxChar * iTarget / 2 - 10, 50, szBuffer, iTarget);

		// 绘制圈s标志
		RoundRect(hdc, cxClient / 2 + cxChar * iTarget / 2 + 10 - 10, 48, cxClient / 2 + cxChar * iTarget / 2 + 30 - 10, 68, 5, 5);
		StringCchCopy(szBuffer, 16, TEXT("S"));
		StringCchLength(szBuffer, 16, &iTarget);
		TextOut(hdc, cxClient / 2 + cxChar * iTarget / 2 + 35 - 10, 50, szBuffer, iTarget);

		// 绘制左边按钮
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		RoundRect(hdc, 98, 155, 102, 170, 2, 5);
		RoundRect(hdc, 98, 195, 102, 235, 2, 5);
		RoundRect(hdc, 98, 245, 102, 285, 2, 5);

		// 绘制右边按钮
		RoundRect(hdc, cxClient - 102, 185, cxClient - 98, 225, 2, 5);

		// 绘制手机框架
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		RoundRect(hdc, 100, 100, cxClient - 100, cyClient - 100, 30, 25);
		
		// 绘制中间屏幕
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, 110, 150, cxClient - 110, cyClient - 150);

		// 绘制上边出口
		Ellipse(hdc, cxClient / 2 - 3, 100 + 10, cxClient / 2 + 3, 100 + 16);
		Ellipse(hdc, cxClient / 2 - 5 - 40, 100 + 10 + 10, cxClient / 2 + 5 - 40, 100 + 20 + 10);
		RoundRect(hdc, cxClient / 2 - 22, 123, cxClient / 2 + 22, 128, 2, 2);

		// 绘制解锁按钮
		Ellipse(hdc, cxClient / 2 - 20, cyClient - 145, cxClient / 2 + 20, cyClient - 105);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		Ellipse(hdc, cxClient / 2 - 18, cyClient - 143, cxClient / 2 + 18, cyClient - 107);


		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}