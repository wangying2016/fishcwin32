/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd);

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

void DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd)
{
	HDC hdc;

	hdc = GetDC(hwnd);

	// ��ɫȡ������������ԭ���ı߿���
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);

	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL fBlocking, fValidBox;
	static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		ptBeg.x = ptEnd.x = GET_X_LPARAM(lParam);
		ptBeg.y = ptEnd.y = GET_Y_LPARAM(lParam);

		SetCapture(hwnd);
		DrawBoxOutline(hwnd, ptBeg, ptEnd);
		SetCursor(LoadCursor(NULL, IDC_CROSS));

		fBlocking = TRUE;
		return 0;

	case WM_MOUSEMOVE:
		if (fBlocking) {
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			// ����һ�ε��߸��ǵ�
			DrawBoxOutline(hwnd, ptBeg, ptEnd);

			ptEnd.x = GET_X_LPARAM(lParam);
			ptEnd.y = GET_Y_LPARAM(lParam);

			// �����µ�λ�õı߿���
			DrawBoxOutline(hwnd, ptBeg, ptEnd);
		}
		return 0;

	case WM_LBUTTONUP:
		if (fBlocking) {
			DrawBoxOutline(hwnd, ptBeg, ptEnd);

			ptBoxBeg = ptBeg;
			ptBoxEnd.x = GET_X_LPARAM(lParam);
			ptBoxEnd.y = GET_Y_LPARAM(lParam);

			ReleaseCapture();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			fBlocking = FALSE;
			fValidBox = TRUE;

			InvalidateRect(hwnd, NULL, TRUE);
		}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		if (fValidBox) {
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
		}

		if (fBlocking) {
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}