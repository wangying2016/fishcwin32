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
	RECT rect;
	static int cxClient, cyClient;
	int iMapMode = 0;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		iMapMode = GetMapMode(hdc);
		SetMapMode(hdc, MM_ANISOTROPIC);

		SetWindowExtEx(hdc, 100, 100, NULL);
		// 为什么要修改SetViewportExtEx函数的第三个参数为负数呢
		// 这是要使 ViewExt / WinExt 的值为负数，从而得到改变坐标值变化方向的效果
		// 其中不管是 MM_ISOTROPIC 还是 MM_ANISOTROPIC 映射模式，都是默认为 MM_TEXT
		// 一样的坐标轴增长方向的，也就是x轴向右，y轴向下，要使y轴默认向上，则需要变号
		SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

		Rectangle(hdc, -100, -100, 100, 100);
		Ellipse(hdc, -80, -80, 80, 80);

		TextOut(hdc, -40, -40, TEXT("LOVE"), 4);
		TextOut(hdc, 40, 40, TEXT("FISHC"), 5);

		MoveToEx(hdc, 0, -100, NULL);
		LineTo(hdc, 0, 100);
		MoveToEx(hdc, -100, 0, NULL);
		LineTo(hdc, 100, 0);

		SetMapMode(hdc, iMapMode);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}