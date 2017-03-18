/* -------------------------------------------------------------------
                    MyWindows.c -- ��������ģ��  
				��Windows ������ƣ�SDK������Ƶ�̳�                    
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

	static int xClient;		// �ͻ����Ŀ��
	static int yClient;		// �ͻ����ĸ߶�
	static int xClientMax;	// �ͻ���������ʾ�������

	static int xChar;		// �����ƽ��ˮƽ���
	static int yChar;		// ����Ĵ�ֱ�߶�
	static int xUpper;		// ����Ĵ�д�ַ���ˮƽ���

	static int xPos;		// ��ǰˮƽ�����������λ��
	static int yPos;		// ��ǰ��ֱ�����������λ��

	int i;					// ѭ��������
	int x, y;				// ˮƽ�ʹ�ֱ������

	int FirstLine;			// ʧЧ����ĵ�һ��(���ػ�ĵ�һ��)
	int LastLine;			// ʧЧ��������һ��(���ػ�����һ��)
	HRESULT hr;
	size_t abcLength;		// �û����abc����ĳ���

	// ����Ҫ��ʾ����
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
		// ����豸�������
		hdc = GetDC(hwnd);

		// �������������Ĵ�С
		GetTextMetrics(hdc, &tm);
		xChar = tm.tmAveCharWidth;
		xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2;
		yChar = tm.tmHeight + tm.tmExternalLeading;

		// �ͷ��豸�������
		ReleaseDC(hwnd, hdc);

		// ���ÿͻ����������
		xClientMax = 48 * xChar + 12 * xUpper;

	case WM_SIZE:
		// ��ÿͻ����ĳߴ�
		yClient = HIWORD(lParam);
		xClient = LOWORD(lParam);

		// ���ô�ֱ��������Χ��ҳ���С
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = LINES - 1;
		si.nPage = yClient / yChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// ����ˮƽ��������Χ��ҳ���С
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + xClientMax / xChar;
		si.nPage = xClient / xChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;

	case WM_HSCROLL:
		// ���ˮƽ��������������Ϣ
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		
		// ���浱ǰ����λ�ã���Щ���бȽ�
		xPos = si.nPos;
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

			// �û���������ұߵĹ�������
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// �û��϶�������
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// ���ù������������λ��
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		// ��ù����������λ�ã����ڴ��ڵ����������ܲ���ͬһ��ֵ
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// ���Ǯ�����ֵ���бȽϣ������ͬ���������
		if (si.nPos != xPos) {
			ScrollWindow(hwnd, xChar * (xPos - si.nPos), 0, NULL, NULL);
		}

	case WM_VSCROLL:
		// ��ô�ֱ��������������Ϣ
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		// ���浱ǰλ�ã���Щ���бȽ�
		yPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// �û��������Home����
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// �û��������End����
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// �û�����������ϵ�������
		case SB_LINEUP:
			si.nPos -= 1;
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

		// ���ù������������λ��
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// ��ù����������λ�ã����ڴ��ڵ����������ܲ���ͬһ��ֵ
		GetScrollInfo(hwnd, SB_VERT, &si);

		// ���ǰ�ı����ֵ���бȽϣ������ͬ���������
		if (si.nPos != yPos) {
			ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_PAINT:
		// ׼������
		hdc = BeginPaint(hwnd, &ps);

		// ��ô�ֱ��������λ��
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		yPos = si.nPos;

		// ���ˮƽ��������λ��
		GetScrollInfo(hwnd, SB_HORZ, &si);
		xPos = si.nPos;

		// ������Ҫ�ػ������
		FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
		LastLine = min(LINES - 1, yPos + ps.rcPaint.bottom / yChar);

		for (i = FirstLine; i <= LastLine; i++) {
			x = xChar * (1 - xPos);
			y = yChar * (i - yPos);

			// ʹ�ð�ȫ�ַ�����������ַ�������
			hr = StringCchLength(abc[i], 55, &abcLength);
			if (FAILED(hr) | (abcLength == NULL)) {
				// ��ȡ�ַ���ʧ����ʾ
			}
			// �ڿͻ�������һ������
			TextOut(hdc, x, y, abc[i], abcLength);
		}

		// �������
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}