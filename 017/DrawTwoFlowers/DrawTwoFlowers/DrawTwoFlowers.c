/* -------------------------------------------------------------------
MyWindows.c -- 基本窗口模型
《Windows 程序设计（SDK）》视频教程
--------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>
#include <strsafe.h>

#define PI 3.1415926                       //圆周率
#define R(theta)  (sin(4 * theta))      //待作图函数【修改此处，可以得到不同的函数图】
#define P_MIN (0)                            //区间左端点 
#define P_MAX (2 * PI)                    //区间右端点【修改此处及上处，可以得到不同的作图区间】
#define LIMIT 5                                //最大坐标轴标度 【修改此处，可以对图像进行缩放】
#define S_NUM 360                         //采样数

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
	static int cxClient, cyClient;
	POINT apt[S_NUM];
	double theta;
	int x, y, cxCenter, cyCenter;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		cxCenter = cxClient / 2;
		cyCenter = cyClient / 2;
		// 绘制右边的那朵花
		x = (int)(R(P_MIN) * cos(P_MIN) * cxCenter / LIMIT + cxCenter);
		y = (int)(-1 * R(P_MIN) * sin(P_MIN) * cyCenter / LIMIT + cyCenter);   //计算初始屏幕坐标
		MoveToEx(hdc, x + 200, y + 100, NULL);

		for (int i = 0; i < S_NUM; i++)
		{
			theta = (double)((P_MAX - P_MIN) * i / S_NUM + P_MIN);  //计算角度

			apt[i].x = (int)(R(theta) * cos(theta) * cxCenter / LIMIT + cxCenter + 200);
			apt[i].y = (int)(-1 * R(theta) * sin(theta) * cyCenter / LIMIT + cyCenter + 100);  //计算屏幕坐标
		}
		PolylineTo(hdc, apt, S_NUM);
		MoveToEx(hdc, x + 200, y + 100, NULL);
		LineTo(hdc, x + 200, cyClient);
		// 绘制左边的那朵花
		x = (int)(R(P_MIN) * cos(P_MIN) * cxCenter / LIMIT + cxCenter);
		y = (int)(-1 * R(P_MIN) * sin(P_MIN) * cyCenter / LIMIT + cyCenter);   //计算初始屏幕坐标
		MoveToEx(hdc, x - 200, y - 100, NULL);

		for (int i = 0; i < S_NUM; i++)
		{
			theta = (double)((P_MAX - P_MIN) * i / S_NUM + P_MIN);  //计算角度

			apt[i].x = (int)(R(theta) * cos(theta) * cxCenter / LIMIT + cxCenter - 200);
			apt[i].y = (int)(-1 * R(theta) * sin(theta) * cyCenter / LIMIT + cyCenter - 100);  //计算屏幕坐标
		}
		PolylineTo(hdc, apt, S_NUM);
		MoveToEx(hdc, x - 200, y - 100, NULL);
		LineTo(hdc, x - 200, cyClient);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}