/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>

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
		
		// ���Ƹ�����
		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);
		MoveToEx(hdc, cxClient / 2, 0, NULL);
		LineTo(hdc, cxClient / 2, cyClient);

		// ���õ�ǰӳ��ģʽΪ MM_LOMETRIC����ÿ���߼���λӳ��Ϊ0.1���ף�x�����ң�y������
		// ���������ڵ�ӳ��ģʽ��ͬ����ѧ�е�����ϵģʽ
		SetMapMode(hdc, MM_LOMETRIC);
		// ָ��ӳ�䵽����ԭ��(0, 0)���豸ԭ������꣬��ָ���ĸ��豸ԭ�㽫ӳ�䵽�߼�ԭ��(0, 0)
		// ������ָ���Ĵ��ڵ��������е�λ��Ϊ���ǵ��߼�ԭ��(0, 0)��ʵ�ʵ��豸�ϵ�ӳ��
		// ����ָ�����Ǵ�������Ϊ����ϵԭ��λ�ã�������ѧϰ��
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		// ���������߾��е�λ�ÿ�ʼ��������ͼ��
		MoveToEx(hdc, -cxClient / 2, 0, NULL);
		for (double x = -cxClient / 2; x < cxClient / 2; x += 0.01) {
			// ������������궼������һ���ֵķŴ�����x += 0.01 ��Ϊ��ϸ������
			LineTo(hdc, x * 200, sin(x) * 300);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}