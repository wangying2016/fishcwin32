/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型  
				《Windows 程序设计（SDK）》视频教程                    
--------------------------------------------------------------------*/

#include <windows.h>
#include <StrSafe.h>

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
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
	TEXTMETRIC tm;
	SCROLLINFO si;

	static int xClient;		// 客户区的宽度
	static int yClient;		// 客户区的高度
	static int xClientMax;	// 客户区内容显示的最大宽度

	static int xChar;		// 字体的平均水平宽度
	static int yChar;		// 字体的垂直高度
	static int xUpper;		// 字体的大写字符的水平宽度

	static int xPos;		// 当前水平滚动条滑块的位置
	static int yPos;		// 当前垂直滚动条滑块的位置

	int i;					// 循环计数器
	int x, y;				// 水平和垂直的坐标

	int FirstLine;			// 失效区域的第一行(需重绘的第一行)
	int LastLine;			// 失效区域的最后一行(需重绘的最后一行)
	HRESULT hr;
	size_t abcLength;		// 用户存放abc数组的长度

	// 创建要显示的行
	#define LINES 28
	static TCHAR *abc[] = {
		TEXT("anteater"), TEXT("bear"), TEXT("cougar"),
		TEXT("dingo"), TEXT("elephant"), TEXT("falcon"),
		TEXT("gazelle"), TEXT("hyena"), TEXT("iguana"),
		TEXT("jackal"), TEXT("kangaroo"), TEXT("llama"),
		TEXT("moose"), TEXT("newt"), TEXT("octopus"),
		TEXT("penguin"), TEXT("quail"), TEXT("rat"),
		TEXT("squid"), TEXT("tortoise"), TEXT("urus"),
		TEXT("vole"), TEXT("walrus"), TEXT("xylophone"),
		TEXT("yak"), TEXT("zebra"),
		TEXT("This line contains words, but no character. Go figure."),
		TEXT("")
	};

	switch (message)
	{
	case WM_CREATE:
		// 获得设备环境句柄
		hdc = GetDC(hwnd);

		// 获得文字中字体的大小
		GetTextMetrics(hdc, &tm);
		xChar = tm.tmAveCharWidth;
		xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2;
		yChar = tm.tmHeight + tm.tmExternalLeading;

		// 释放设备环境句柄
		ReleaseDC(hwnd, hdc);

		// 设置客户区的最大宽度
		xClientMax = 48 * xChar + 12 * xUpper;

	case WM_SIZE:
		// 获得客户区的尺寸
		yClient = HIWORD(lParam);
		xClient = LOWORD(lParam);

		// 设置垂直滚动条范围和页面大小
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = LINES - 1;
		si.nPage = yClient / yChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// 设置水平滚动条范围和页面大小
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + xClientMax / xChar;
		si.nPage = xClient / xChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;

	case WM_HSCROLL:
		// 获得水平滚动条的所有信息
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		
		// 保存当前滑块位置，迟些进行比较
		xPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// 用户点击滚动条左边的三角形
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

			// 用户点击滚动条右边的三角形
		case SB_LINERIGHT:
			si.nPos += 1;
			break;

			// 用户点击滑块右边的滚动条轴
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// 用户拖动滚动条
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// 设置滚动条滑块的新位置
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		// 获得滚动条滑块的位置，由于窗口调整，它可能不是同一个值
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// 与此钱保存的值进行比较，如果不同则滚动窗口
		if (si.nPos != xPos) {
			ScrollWindow(hwnd, xChar * (xPos - si.nPos), 0, NULL, NULL);
		}

	case WM_VSCROLL:
		// 获得垂直滚动条的所有信息
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		// 保存当前位置，迟些进行比较
		yPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// 用户点击键盘Home按键
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// 用户点击键盘End按键
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// 用户点击滚动条上的三角形
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// 用户点击滑块上边的滚动条轴
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// 用户点击滑块下边的滚动条轴
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// 用户拖动滚动条
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// 设置滚动条滑块的新位置
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// 获得滚动条滑块的位置，由于窗口调整，它可能不是同一个值
		GetScrollInfo(hwnd, SB_VERT, &si);

		// 与此前的保存的值进行比较，如果不同则滚动窗口
		if (si.nPos != yPos) {
			ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_PAINT:
		// 准备绘制
		hdc = BeginPaint(hwnd, &ps);

		// 获得垂直滚动条的位置
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		yPos = si.nPos;

		// 获得水平滚动条的位置
		GetScrollInfo(hwnd, SB_HORZ, &si);
		xPos = si.nPos;

		// 计算需要重绘的区域
		FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
		LastLine = min(LINES - 1, yPos + ps.rcPaint.bottom / yChar);

		for (i = FirstLine; i <= LastLine; i++) {
			x = xChar * (1 - xPos);
			y = yChar * (i - yPos);

			// 使用安全字符串函数获得字符串长度
			hr = StringCchLength(abc[i], 55, &abcLength);
			if (FAILED(hr) | (abcLength == NULL)) {
				// 获取字符串失败提示
			}
			// 在客户区绘制一行数据
			TextOut(hdc, x, y, abc[i], abcLength);
		}

		// 绘制完毕
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}