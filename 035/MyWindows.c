/* -------------------------------------------------------------------
					MyWindows.c -- 基本窗口模型
				《Windows 程序设计（SDK）》视频教程
--------------------------------------------------------------------*/

#include <windows.h>

#define BUFFER(x, y) *(pBuffer + y * cxBuffer + x)

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
		TEXT("文本编辑器"), 
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
	// cxChar 字符的平均宽度
	// cyChar 字符的平均高度
	// cxClient 客户区的宽度
	// cyClient 客户区的高度
	// cxBuffer 窗口横向最大缓冲区
	// cyBuffer 窗口纵向最大缓冲区
	// xCaret 输入插入符号的横坐标
	// yCaret 输入插入符号的纵坐标
	static int cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, xCaret, yCaret;
	// pBuffer 存储整个屏幕的内容的缓冲区
	static TCHAR *pBuffer = NULL;
	// 设备环境
	HDC hdc;
	// x是横坐标计数，y是纵坐标技术，i是临时计数
	int x, y, i;
	// 描述客户区绘制的信息
	PAINTSTRUCT ps;
	// 当前设备环境中字体的信息
	TEXTMETRIC tm;

	switch (message)
	{
	// 窗口创建时，计算字体的平均宽度和高度
    // 得到 cxChar 和 cyChar 的值
	case WM_CREATE:
		hdc = GetDC(hwnd);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;

		ReleaseDC(hwnd, hdc);

		// 此处木有返回，木有break

	case WM_SIZE:
		// 获得客户区的宽度和高度
		// 得到 cxClient 和 cyClient 的值
		if (message == WM_SIZE) {
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}
		// 获得横向最大缓存区和纵向最大缓存区的值
		// 得到 cxBuffer 和 cyBuffer
		cxBuffer = max(1, cxClient / cxChar);
		cyBuffer = max(1, cyClient / cyChar);

		// 分配足够整个客户区显示的字符缓存区的空间
		if (pBuffer != NULL) {
			free(pBuffer);
		}
		pBuffer = (TCHAR *)malloc(cxBuffer * cyBuffer * sizeof(TCHAR));

		// 将整个字符缓存区的空间置为空字符
		for (y = 0; y < cyBuffer; y++) {
			for (x = 0; x < cxBuffer; x++) {
				BUFFER(x, y) = ' ';
			}
		}

		// 将插入符号指向左上角
		xCaret = 0;
		yCaret = 0;

		// 如果当前窗口获得了输入焦点，则设置输入焦点
		if (hwnd == GetFocus()) {
			// 输入焦点在指定位置
			// 位置坐标为 (xCaret, yCaret)
			SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		}

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	// 创建、设置输入插入符号并且显示
	case WM_SETFOCUS:
		CreateCaret(hwnd, NULL, cxChar, cyChar);
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		ShowCaret(hwnd);
		return 0;
	
	// 隐藏并摧毁输入插入符号
	// 这里的隐藏操作是必要的，只有在 ShowCaret() 与 HideCaret()
	// 数量一一对应的时候，输入插入符号才会显示出来
	case WM_KILLFOCUS:
		HideCaret(hwnd);
		DestroyCaret();
		return 0;

	// 处理击键消息
	case WM_KEYDOWN:
		switch (wParam)
		{
		// home键
		case VK_HOME:
			xCaret = 0;
			break;

		// end键
		case VK_END:
			xCaret = cxBuffer - 1;
			break;

		// pg up键
		case VK_PRIOR:
			yCaret = 0;
			break;

		// pg dn键
		case VK_NEXT:
			yCaret = cyBuffer - 1;
			break;

		// <-键
		case VK_LEFT:
			xCaret = max(xCaret - 1, 0);
			break;

		// ->键
		case VK_RIGHT:
			xCaret = min(xCaret + 1, cxBuffer - 1);
			break;

		// 上键
		case VK_UP:
			yCaret = max(yCaret - 1, 0);
			break;

		// 下键
		case VK_DOWN:
			yCaret = min(yCaret + 1, cyBuffer - 1);
			break;

		// del键
		case VK_DELETE:
			// 要删除指定位置的一个字符，即要把后面的字符
			// 一个一个挪到前面一个位置上，再将最后一个位置
			// 的字符置为空
			for (x = xCaret; x < cxBuffer - 1; x++) {
				BUFFER(x, yCaret) = BUFFER(x + 1, yCaret);
			}
			BUFFER(cxBuffer - 1, yCaret) = ' ';
			HideCaret(hwnd);

			hdc = GetDC(hwnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			TextOut(hdc, xCaret * cxChar, yCaret * cyChar, &BUFFER(xCaret, yCaret), cxBuffer - xCaret);
			ReleaseDC(hwnd, hdc);
			ShowCaret(hwnd);
			break;
		}

		SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		return 0;

	// 处理字符消息
	case WM_CHAR: // lParam 表示重复次数， wParam 表示字符编码
		for (i = 0; i < (int)LOWORD(lParam); i++) {
			switch (wParam)
			{
			// backspace键
			case '\b':
				if (xCaret > 0) {
					xCaret--;
					SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, 1);
				}
				break;
			
			// tab键
			case '\t':
				do {
					SendMessage(hwnd, WM_CHAR, ' ', 1);
				} while (xCaret % 8 != 0);
				break;

			// enter键
			case '\n':
				if (++yCaret == cyBuffer) {
					yCaret = 0;
				}
				break;

			// enter键
			case '\r':
				xCaret = 0;
				if (++yCaret == cyBuffer) {
					yCaret = 0;
				}
				break;

			// esc键
			case '\x1B': // 十六进制的1B，对应的ASCII字符是ESC
				for (y = 0; y < cyBuffer; y++) {
					for (x = 0; x < cxBuffer; x++) {
						BUFFER(x, y) = ' ';
					}
				}

				xCaret = 0;
				yCaret = 0;

				InvalidateRect(hwnd, NULL, FALSE);
				break;

			// 输出用户按下的键位
			default:
				BUFFER(xCaret, yCaret) = (TCHAR)wParam;

				HideCaret(hwnd);
				hdc = GetDC(hwnd);
				SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
				TextOut(hdc, xCaret * cxChar, yCaret * cyChar, &BUFFER(xCaret, yCaret), 1);
				ReleaseDC(hwnd, hdc);
				ShowCaret(hwnd);

				// 本行输完了，则跳转下一行开头显示
				if (++xCaret == cxBuffer) {
					xCaret = 0;
					if (++yCaret == cyBuffer) {
						yCaret = 0;
					}
				}
				break;
			}
			SetCaretPos(xCaret * cxChar, yCaret * cyChar);
			return 0;
		}

	// 显示所有行的信息，之前都是以每行为单位显示的
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		for (y = 0; y < cyBuffer; y++) {
			TextOut(hdc, 0, y * cyChar, &BUFFER(0, y), cxBuffer);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}