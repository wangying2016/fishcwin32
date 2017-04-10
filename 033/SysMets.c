/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型
                《Windows 程序设计（SDK）》视频教程
--------------------------------------------------------------------*/

#include <windows.h>
#include "strsafe.h"
#include "sysmets.h"

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
	TCHAR szBuffer[10];
	static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
	size_t iTarget;
	int i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);

		// 设置客户区的最大宽度 
		// （我们这里设置为 22 个大写字符的宽度 + 40 个小写字符的宽度）
		iMaxWidth = 22 * cxCaps + 40 * cxChar;
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		// 设置垂直滚动条
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// 设置水平滚动条
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + iMaxWidth / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;

	case WM_VSCROLL:
		// 获得滚动条信息
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		// 获得滑块的位置
		iVertPos = si.nPos;

		// 根据消息的通知码改变滚动条滑块的位置
		switch (LOWORD(wParam))
		{
			// 用户点击键盘 Home 按键
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// 用户点击键盘 End 按键
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// 用户点击滚动条上边的三角形
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// 用户点击滚动条下边的三角形
		case SB_LINEDOWN:
			si.nPos += 1;
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

		// 设置改变后的滚动条滑块位置
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// 获得滚动条滑块的位置，由于窗口调整，它可能不是同一个值
		GetScrollInfo(hwnd, SB_VERT, &si);

		// 与此前的保存的值进行比较，如果不同则滚动窗口
		if (si.nPos != iVertPos)
		{
			ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_HSCROLL:
		// 获得滚动条信息
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// 获得滑块的位置
		iHorzPos = si.nPos;

		// 根据消息的通知码改变滚动条滑块的位置
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

			// 用户点击滑块左边的滚动条轴
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

			// 用户点击滑块右边的滚动条轴
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// 滚动条最终停留的位置
		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// 设置改变后的滚动条滑块位置
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		// 获得滚动条滑块的位置，由于窗口调整，它可能不是同一个值
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// 与此前的保存的值进行比较，如果不同则滚动窗口
		if (si.nPos != iHorzPos)
		{
			ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		// 获得垂直滚动条的位置
		si.cbSize = sizeof (si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;

		// 获得水平滚动条的位置
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		// 计算需要重绘的区域并确保范围在[0, NUMLINES-1]之中
		iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

		for (i = iPaintBeg; i <= iPaintEnd; i++)
		{
			// 计算此时重绘无效区域的位置
			x = cxChar * (1 - iHorzPos);
			y = cyChar * (i - iVertPos);

			StringCchLength(sysmetrics[i].szLabel, 1024, &iTarget);
			TextOut(hdc, x, y, sysmetrics[i].szLabel, iTarget);

			StringCchLength(sysmetrics[i].szDesc, 1024, &iTarget);
			TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, iTarget);

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			StringCchPrintf(szBuffer, 10, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex));
			StringCchLength(szBuffer, 10, &iTarget);
			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, iTarget);

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_DOWN:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;
		case VK_UP:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
			break;
		case VK_HOME:
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
			break;
		case VK_END:
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;
		case VK_PRIOR:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;
		case VK_NEXT:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}