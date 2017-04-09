/* -------------------------------------------------------------------
MyWindows.c -- ��������ģ��
��Windows ������ƣ�SDK������Ƶ�̳�
--------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>

#define NUM		1000
#define TWOPI  (2 * 3.14159)

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
	POINT apt[NUM];

	int i;
	int iMapMode;
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		// ���Ƹ�����
		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);
		MoveToEx(hdc, cxClient / 2, 0, NULL);
		LineTo(hdc, cxClient / 2, cyClient);

		iMapMode = GetMapMode(hdc);

		// ����ӳ��ģʽ
		SetMapMode(hdc, MM_LOMETRIC);
		SetViewportOrgEx(hdc, 0, cyClient / 2, NULL);

		// ����ǰ�ͻ��˵��豸����ת�����߼�����
		GetClientRect(hwnd, &rect);
		DPtoLP(hdc, (PPOINT)&rect, 2);

		for (i = 0; i < NUM; i++)
		{
			// ʹ���߼������������λ��
			apt[i].x = i * (rect.right - rect.left) / NUM;
			apt[i].y = (int)(rect.top * sin(TWOPI * i / NUM));
		}

		// ��������ͼ��
		MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
		PolylineTo(hdc, apt, NUM);

		// �ָ�ԭ����ӳ��ģʽ
		SetMapMode(hdc, iMapMode);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}