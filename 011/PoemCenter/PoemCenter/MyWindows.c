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
	static int cyChar,cxChar;
	static int cyClient, cxClient;
	TEXTMETRIC tm;
	static TCHAR *poetry[] = {
		TEXT("I Think I Can"),
		TEXT(""),
		TEXT("If you think you are beaten, you are;"),
		TEXT("If you think you dare not, you don't;"),
		TEXT("If you want to win but think you can't;"),
		TEXT("It's almost a cinch you won't."),
		TEXT("If you think you'll lose, you're lost;"),
		TEXT("For out of the world we find;"),
		TEXT("Success begins with a fellow's will;"),
		TEXT("It's all in a state of mind."),
		TEXT("Life's battles don't always go;"),
		TEXT("To the stronger and faster man;"),
		TEXT("But sooner or later the man who wins;"),
		TEXT("Is the man who thinks he can.")
	};


	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		cxChar = tm.tmAveCharWidth;
		ReleaseDC(hwnd, hdc);

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		// 获取最长行的长度和总行数
		int totalCount = sizeof(poetry) / sizeof(poetry[0]);
		int maxLength = 0;
		for (int count = 0; count < totalCount; ++count) {
			int	tempLength = maxLength;
			StringCchLength(poetry[count], 256, &tempLength);
			if (tempLength > maxLength) {
				maxLength = tempLength;
			}
		}
		// 计算起始高度
		int startY = (rect.bottom - rect.top - totalCount * cyChar) / 2;
		int startX = (rect.right - rect.left - maxLength * cxChar) / 2;
		// 打印诗词
		for (int item = 0; item < totalCount; ++item) {
			size_t iTarget = 0;
			StringCchLength(poetry[item], 256, &iTarget);
			if (item == 0) {
				SetTextAlign(hdc, TA_CENTER);
				TextOut(hdc, (rect.right - rect.left) / 2, startY, poetry[item], iTarget);
			}
			else {
				SetTextAlign(hdc, TA_LEFT);
				startY += cyChar;
				TextOut(hdc, startX, startY, poetry[item], iTarget);
			}
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}