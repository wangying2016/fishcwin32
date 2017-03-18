/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��
                ��Windows ������ƣ�SDK������Ƶ�̳�
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
		MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,
		TEXT("��C������"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
	TCHAR szBuffer[10];
	static int cxChar, cxCaps, cyChar, iVertScroll;
	static int cxClient, cyClient;
	size_t iTarget;
	int i;
	int y;
	TCHAR szABuffer[128];

	SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = LOWORD(lParam);
		return 0;

	case WM_VSCROLL:
		hdc = GetDC(hwnd);
		switch (LOWORD(wParam)) 
		{
		case SB_LINEUP:
			iVertScroll += 1;
			break;

		case SB_LINEDOWN:
			iVertScroll -= 1;
			break;

		case SB_PAGEUP:
			iVertScroll += cyClient / cyChar;
			break;

		case SB_PAGEDOWN:
			iVertScroll -= cyClient / cyChar;
			break;

		case SB_THUMBPOSITION:
			iVertScroll = HIWORD(wParam);
			break;
		}
		SetScrollPos(hwnd, SB_VERT, iVertScroll, TRUE);
		iVertScroll = max(0, min(NUMLINES, iVertScroll));
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
		return 0;

	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			y = (i - iVertScroll) * cyChar;
			StringCchLength(sysmetrics[i].szLabel, 1024, &iTarget);
			TextOut(hdc, 0, y, sysmetrics[i].szLabel, iTarget);

			StringCchLength(sysmetrics[i].szDesc, 1024, &iTarget);
			TextOut(hdc, 22 * cxCaps, y, sysmetrics[i].szDesc, iTarget);

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			StringCchPrintf(szBuffer, 10, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex));
			StringCchLength(szBuffer, 10, &iTarget);
			TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer, iTarget);

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}
		StringCchPrintf(szABuffer, 128, TEXT("��ǰλ�ã�%d/%d"), iVertScroll, NUMLINES);
		StringCchLength(szABuffer, 128, &iTarget);
		if (22 * cxCaps + 40 * cxChar + iTarget * cxChar + 90 < cxClient) {
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			TextOut(hdc, cxClient - 30, 10, szABuffer, iTarget);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}