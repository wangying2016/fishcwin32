/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>

#define TWOPI (2 * 3.14159)

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
	static HRGN hRgn[6];
	static HRGN hClipRgn;
	static int cxClient, cyClient;
	double fRadius = 0.0;
	double fAngle = 0.0;
	int i;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		// ���������������Բ����
		hRgn[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		hRgn[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		// ���������������Բ����
		hRgn[2] = CreateEllipticRgn(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
		hRgn[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);

		// ��ʼ��ʣ��� HRGN
		hRgn[4] = CreateRectRgn(0, 0, 1, 1);
		hRgn[5] = CreateRectRgn(0, 0, 1, 1);
		hClipRgn = CreateRectRgn(0, 0, 1, 1);

		// ��ʼ�ϲ�����
		CombineRgn(hRgn[4], hRgn[0], hRgn[1], RGN_OR);
		CombineRgn(hRgn[5], hRgn[2], hRgn[3], RGN_OR);
		CombineRgn(hClipRgn, hRgn[4], hRgn[5], RGN_XOR);

		// �������������Ѿ��ò����ˣ��������
		for (i = 0; i < 6; ++i) {
			DeleteObject(hRgn[i]);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		// ���ô�����������ӿڵ�ӳ��
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		// ���õ�ǰ�豸�����ļ�������
		SelectClipRgn(hdc, hClipRgn);

		// �������ı߿��Ա㿴��Ч��
		FrameRgn(hdc, hClipRgn, (HBRUSH)GetStockObject(BLACK_BRUSH), 1, 1);
		
		// �������ε�����ֱ�Ǳ���б�ߵĳ���
		fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);
		for (fAngle = 0.0; fAngle < TWOPI; fAngle += TWOPI / 360) {
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, (int)(fRadius * cos(fAngle) + 0.5), (int)(-fRadius * sin(fAngle) + 0.5));
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		// �������Ҫ�˵� HRGN 
		DeleteObject(hClipRgn);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}