/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型  
				《Windows 程序设计（SDK）》视频教程                    
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
		
		// 绘制辅助线
		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);
		MoveToEx(hdc, cxClient / 2, 0, NULL);
		LineTo(hdc, cxClient / 2, cyClient);

		// 设置当前映射模式为 MM_LOMETRIC，即每个逻辑单位映射为0.1毫米，x轴向右，y轴向上
		// 这样将窗口的映射模式类同于数学中的坐标系模式
		SetMapMode(hdc, MM_LOMETRIC);
		// 指定映射到窗口原点(0, 0)的设备原点的坐标，即指定哪个设备原点将映射到逻辑原点(0, 0)
		// 这里是指定的窗口的最左侧居中的位置为我们的逻辑原点(0, 0)在实际的设备上的映射
		// 这里指定的是窗口中心为坐标系原点位置，符合数学习惯
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		// 这里从最左边居中的位置开始绘制正弦图像
		MoveToEx(hdc, -cxClient / 2, 0, NULL);
		for (double x = -cxClient / 2; x < cxClient / 2; x += 0.01) {
			// 这里的两个坐标都进行了一部分的放大，其中x += 0.01 是为了细分曲线
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