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
	RECT rect;
	TEXTMETRIC tm;
	static int cxAveChar, cyChar;
	TCHAR szBuffer[][128] = { TEXT("North"), TEXT("West"), TEXT("Center"), TEXT("East"), TEXT("South") };

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxAveChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		int textX = 0;
		int textY = 0;
		size_t iTarget = 0;
		// 绘制North
		SetTextAlign(hdc, TA_TOP | TA_CENTER);
		StringCchLength(szBuffer[0], 128, &iTarget);
		textX = (rect.right - rect.left) / 2;
		textY = cxAveChar;
		TextOut(hdc, textX, textY, szBuffer[0], iTarget);
		// 绘制West
		SetTextAlign(hdc, TA_LEFT);
		StringCchLength(szBuffer[1], 128, &iTarget);
		textX = cxAveChar;
		textY = (rect.bottom - rect.top - cyChar) / 2;
		TextOut(hdc, textX, textY, szBuffer[1], iTarget);
		// 绘制East
		SetTextAlign(hdc, TA_RIGHT);
		StringCchLength(szBuffer[3], 128, &iTarget);
		textX = rect.right - rect.left - cxAveChar;
		textY = (rect.bottom - rect.top - cyChar) / 2;
		TextOut(hdc, textX, textY, szBuffer[3], iTarget);
		// 绘制South
		SetTextAlign(hdc, TA_BOTTOM | TA_CENTER);
		StringCchLength(szBuffer[4], 128, &iTarget);
		textX = (rect.right - rect.left) / 2;
		textY = rect.bottom - rect.top - cyChar - cxAveChar;
		TextOut(hdc, textX, textY, szBuffer[4], iTarget);
		// 绘制Center
		SetTextAlign(hdc, TA_CENTER);
		StringCchLength(szBuffer[2], 128, &iTarget);
		textX = (rect.right - rect.left) / 2;
		textY = (rect.bottom - rect.top - cyChar) / 2;
		TextOut(hdc, textX, textY, szBuffer[2], iTarget);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}