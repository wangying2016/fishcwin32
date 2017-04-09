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

		iMapMode = GetMapMode(hdc);
		SetMapMode(hdc, MM_ISOTROPIC);

		// ���ô��ڵ�ˮƽ��Χ�ʹ�ֱ��Χ��Ϊ100,
		SetWindowExtEx(hdc, 100, 100, NULL);

		// �ոմ��ڵ�100�߼���Χ�൱���ӿڵ�1/2��Ⱥ͸߶ȣ�
		// Ҳ����˵�������ͻ���Ӧ���ǳ��Ϳ���200�߼���λ
		SetViewportExtEx(hdc, cxClient / 2, cyClient / 2, NULL);

		// ���ô��ڵ�ԭ�����ӿڵ��м�λ����ʾ
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

		// �ڿͻ������м仭һ���뾶��80��Բ
		Ellipse(hdc, -80, -80, 80, 80);

		SetMapMode(hdc, iMapMode);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}