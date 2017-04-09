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
	static RECT rect, rectC, rectCC;
	HBRUSH hBrush, hOldBrush;
	static int cxClient, cyClient;
	POINT point;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		if (PtInRect(&rectC, point)) {
			MessageBox(hwnd, TEXT("被点中了呢！"), TEXT("FishC"), MB_OK);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		 
		/*rect.left = cxClient / 2 - 50;
		rect.right = cxClient / 2 + 50;
		rect.top = cyClient / 2 + 50;
		rect.bottom = cyClient / 2 - 50;*/

		// 使用SetRect函数
		SetRect(&rect, cxClient / 2 - 50, cyClient / 2 + 50, cxClient / 2 + 50, cyClient / 2 - 50); 

		// 使用Rectanble函数
		/*hBrush = CreateSolidBrush(RGB(255, 0, 0));
		hOldBrush = SelectObject(hdc, hBrush);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, hOldBrush);*/

		// 使用FillRect函数
		/*hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, hBrush);*/

		// 使用FrameRect函数
		/*hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FrameRect(hdc, &rect, hBrush);*/

		// 使用InvertRect函数
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, hBrush);
		InvertRect(hdc, &rect);

		// 使用OffsetRect函数，移动需要在绘制之前
		/*OffsetRect(&rect, -50, -50);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, hBrush);*/


		// 使用InflateRect函数
		SetRect(&rectC, cxClient / 2 + 100, cyClient / 2 - 50, cxClient / 2 + 200, cyClient / 2 + 50);	
		InflateRect(&rectC, 100, 100);
		FillRect(hdc, &rectC, hBrush);

		// 使用SetRectEmpty
		/*SetRectEmpty(&rectC);*/

		// 使用CopyRect
		/*RECT rectDst;
		CopyRect(&rectDst, &rectC);*/

		// 使用IntersectRect，两个矩形的交集
		/*RECT rectIntersect;
		IntersectRect(&rectIntersect, &rect, &rectC);*/

		// 使用UnionRect函数，两个矩形的并集
		/*UnionRect(&rectCC, &rect, &rectC);
		FrameRect(hdc, &rectCC, hBrush);*/

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}