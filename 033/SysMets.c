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

		// ���ÿͻ���������� 
		// ��������������Ϊ 22 ����д�ַ��Ŀ�� + 40 ��Сд�ַ��Ŀ�ȣ�
		iMaxWidth = 22 * cxCaps + 40 * cxChar;
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		// ���ô�ֱ������
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// ����ˮƽ������
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + iMaxWidth / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;

	case WM_VSCROLL:
		// ��ù�������Ϣ
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		// ��û����λ��
		iVertPos = si.nPos;

		// ������Ϣ��֪ͨ��ı�����������λ��
		switch (LOWORD(wParam))
		{
			// �û�������� Home ����
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// �û�������� End ����
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// �û�����������ϱߵ�������
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// �û�����������±ߵ�������
		case SB_LINEDOWN:
			si.nPos += 1;
			break;

			// �û���������ϱߵĹ�������
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// �û���������±ߵĹ�������
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// �û��϶�������
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// ���øı��Ĺ���������λ��
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// ��ù����������λ�ã����ڴ��ڵ����������ܲ���ͬһ��ֵ
		GetScrollInfo(hwnd, SB_VERT, &si);

		// ���ǰ�ı����ֵ���бȽϣ������ͬ���������
		if (si.nPos != iVertPos)
		{
			ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_HSCROLL:
		// ��ù�������Ϣ
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// ��û����λ��
		iHorzPos = si.nPos;

		// ������Ϣ��֪ͨ��ı�����������λ��
		switch (LOWORD(wParam))
		{
			// �û������������ߵ�������
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

			// �û�����������ұߵ�������
		case SB_LINERIGHT:
			si.nPos += 1;
			break;

			// �û����������ߵĹ�������
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

			// �û���������ұߵĹ�������
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// ����������ͣ����λ��
		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// ���øı��Ĺ���������λ��
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		// ��ù����������λ�ã����ڴ��ڵ����������ܲ���ͬһ��ֵ
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// ���ǰ�ı����ֵ���бȽϣ������ͬ���������
		if (si.nPos != iHorzPos)
		{
			ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		// ��ô�ֱ��������λ��
		si.cbSize = sizeof (si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;

		// ���ˮƽ��������λ��
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		// ������Ҫ�ػ������ȷ����Χ��[0, NUMLINES-1]֮��
		iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

		for (i = iPaintBeg; i <= iPaintEnd; i++)
		{
			// �����ʱ�ػ���Ч�����λ��
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