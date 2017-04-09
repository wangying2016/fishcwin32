/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型  
				《Windows 程序设计（SDK）》视频教程                    
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

		// 创建两个横向的椭圆区域
		hRgn[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		hRgn[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		// 创建两个纵向的椭圆区域
		hRgn[2] = CreateEllipticRgn(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
		hRgn[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);

		// 初始化剩余的 HRGN
		hRgn[4] = CreateRectRgn(0, 0, 1, 1);
		hRgn[5] = CreateRectRgn(0, 0, 1, 1);
		hClipRgn = CreateRectRgn(0, 0, 1, 1);

		// 开始合并区域
		CombineRgn(hRgn[4], hRgn[0], hRgn[1], RGN_OR);
		CombineRgn(hRgn[5], hRgn[2], hRgn[3], RGN_OR);
		CombineRgn(hClipRgn, hRgn[4], hRgn[5], RGN_XOR);

		// 其他六个区域已经用不到了，立马清除
		for (i = 0; i < 6; ++i) {
			DeleteObject(hRgn[i]);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		// 设置窗口坐标对于视口的映射
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		// 设置当前设备环境的剪裁区域
		SelectClipRgn(hdc, hClipRgn);

		// 描绘区域的边框，以便看到效果
		FrameRgn(hdc, hClipRgn, (HBRUSH)GetStockObject(BLACK_BRUSH), 1, 1);
		
		// 由三角形的两条直角边求到斜边的长度
		fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);
		for (fAngle = 0.0; fAngle < TWOPI; fAngle += TWOPI / 360) {
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, (int)(fRadius * cos(fAngle) + 0.5), (int)(-fRadius * sin(fAngle) + 0.5));
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		// 清除不需要了的 HRGN 
		DeleteObject(hClipRgn);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}