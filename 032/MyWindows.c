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
	RECT rect;
	static int cxClient, cyClient;
	static HRGN hRgn[6];
	static HRGN hTargetRgn;
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
		// 创建空区域，用于存放待会儿合并后的组合区域
		hRgn[4] = CreateRectRgn(0, 0, 1, 1);
		hRgn[5] = CreateRectRgn(0, 0, 1, 1);
		// 给 hTargetRgn 初始化
		hTargetRgn = CreateRectRgn(0, 0, 1, 1);

		// 合并两个横向的区域
		CombineRgn(hRgn[4], hRgn[0], hRgn[1], RGN_OR);
		// 合并两个纵向的区域
		CombineRgn(hRgn[5], hRgn[2], hRgn[3], RGN_OR);
		
		// 剔除重叠的部分（四个椭圆），使用 RGN_XOR
		CombineRgn(hTargetRgn, hRgn[4], hRgn[5], RGN_XOR);	

		// 由于所有的区域都合并到了 hTargetRgn 里面去了，其他的所有区域都不需要了
		for (i = 0; i < 6; ++i) {
			DeleteObject(hRgn[i]);	
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		// 使用 FrameRgn 函数
		//for (i = 0; i < 4; ++i) {
		//	// 然后我们填充四个椭圆区域，用黑色画刷
		//	FrameRgn(hdc, hRgn[i], (HBRUSH)GetStockObject(BLACK_BRUSH), 1, 1);
		//}

		// 这里还是显示区域的边框，待会儿好看效果
		FrameRgn(hdc, hTargetRgn, (HBRUSH)GetStockObject(BLACK_BRUSH), 1, 1);
		// 将区域作为当前设备环境的剪裁区域
		SelectClipRgn(hdc, hTargetRgn);
		// 然后我们从客户区的左边开始绘制竖线，每隔10个像素点绘制一条
		for (i = 0; i < cxClient; i += 10) {
			MoveToEx(hdc, i, 0, NULL);
			LineTo(hdc, i, cyClient);  
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		// 删除我们不用的区域，区域也是一个GDI对象
		/*for (i = 0; i < 4; ++i) {
			DeleteObject(hRgn[i]);
		}*/
		DeleteObject(hTargetRgn);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}