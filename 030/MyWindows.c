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
			MessageBox(hwnd, TEXT("���������أ�"), TEXT("FishC"), MB_OK);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		 
		/*rect.left = cxClient / 2 - 50;
		rect.right = cxClient / 2 + 50;
		rect.top = cyClient / 2 + 50;
		rect.bottom = cyClient / 2 - 50;*/

		// ʹ��SetRect����
		SetRect(&rect, cxClient / 2 - 50, cyClient / 2 + 50, cxClient / 2 + 50, cyClient / 2 - 50); 

		// ʹ��Rectanble����
		/*hBrush = CreateSolidBrush(RGB(255, 0, 0));
		hOldBrush = SelectObject(hdc, hBrush);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, hOldBrush);*/

		// ʹ��FillRect����
		/*hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, hBrush);*/

		// ʹ��FrameRect����
		/*hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FrameRect(hdc, &rect, hBrush);*/

		// ʹ��InvertRect����
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, hBrush);
		InvertRect(hdc, &rect);

		// ʹ��OffsetRect�������ƶ���Ҫ�ڻ���֮ǰ
		/*OffsetRect(&rect, -50, -50);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, hBrush);*/


		// ʹ��InflateRect����
		SetRect(&rectC, cxClient / 2 + 100, cyClient / 2 - 50, cxClient / 2 + 200, cyClient / 2 + 50);	
		InflateRect(&rectC, 100, 100);
		FillRect(hdc, &rectC, hBrush);

		// ʹ��SetRectEmpty
		/*SetRectEmpty(&rectC);*/

		// ʹ��CopyRect
		/*RECT rectDst;
		CopyRect(&rectDst, &rectC);*/

		// ʹ��IntersectRect���������εĽ���
		/*RECT rectIntersect;
		IntersectRect(&rectIntersect, &rect, &rectC);*/

		// ʹ��UnionRect�������������εĲ���
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