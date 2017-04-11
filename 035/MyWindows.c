/* -------------------------------------------------------------------
					MyWindows.c -- ��������ģ��
				��Windows ������ƣ�SDK������Ƶ�̳�
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
		MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, 
		TEXT("�ı��༭��"), 
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
	// cxChar �ַ���ƽ�����
	// cyChar �ַ���ƽ���߶�
	// cxClient �ͻ����Ŀ��
	// cyClient �ͻ����ĸ߶�
	// cxBuffer ���ں�����󻺳���
	// cyBuffer ����������󻺳���
	// xCaret ���������ŵĺ�����
	// yCaret ���������ŵ�������
	static int cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, xCaret, yCaret;
	// pBuffer �洢������Ļ�����ݵĻ�����
	static TCHAR *pBuffer = NULL;
	// �豸����
	HDC hdc;
	// x�Ǻ����������y�������꼼����i����ʱ����
	int x, y, i;
	// �����ͻ������Ƶ���Ϣ
	PAINTSTRUCT ps;
	// ��ǰ�豸�������������Ϣ
	TEXTMETRIC tm;

	switch (message)
	{
	// ���ڴ���ʱ�����������ƽ����Ⱥ͸߶�
    // �õ� cxChar �� cyChar ��ֵ
	case WM_CREATE:
		hdc = GetDC(hwnd);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;

		ReleaseDC(hwnd, hdc);

		// �˴�ľ�з��أ�ľ��break

	case WM_SIZE:
		// ��ÿͻ����Ŀ�Ⱥ͸߶�
		// �õ� cxClient �� cyClient ��ֵ
		if (message == WM_SIZE) {
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}
		// ��ú�����󻺴�����������󻺴�����ֵ
		// �õ� cxBuffer �� cyBuffer
		cxBuffer = max(1, cxClient / cxChar);
		cyBuffer = max(1, cyClient / cyChar);

		// �����㹻�����ͻ�����ʾ���ַ��������Ŀռ�
		if (pBuffer != NULL) {
			free(pBuffer);
		}
		pBuffer = (TCHAR *)malloc(cxBuffer * cyBuffer * sizeof(TCHAR));

		// �������ַ��������Ŀռ���Ϊ���ַ�
		for (y = 0; y < cyBuffer; y++) {
			for (x = 0; x < cxBuffer; x++) {
				BUFFER(x, y) = ' ';
			}
		}

		// ���������ָ�����Ͻ�
		xCaret = 0;
		yCaret = 0;

		// �����ǰ���ڻ�������뽹�㣬���������뽹��
		if (hwnd == GetFocus()) {
			// ���뽹����ָ��λ��
			// λ������Ϊ (xCaret, yCaret)
			SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		}

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	// �������������������Ų�����ʾ
	case WM_SETFOCUS:
		CreateCaret(hwnd, NULL, cxChar, cyChar);
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		ShowCaret(hwnd);
		return 0;
	
	// ���ز��ݻ�����������
	// ��������ز����Ǳ�Ҫ�ģ�ֻ���� ShowCaret() �� HideCaret()
	// ����һһ��Ӧ��ʱ�����������ŲŻ���ʾ����
	case WM_KILLFOCUS:
		HideCaret(hwnd);
		DestroyCaret();
		return 0;

	// ���������Ϣ
	case WM_KEYDOWN:
		switch (wParam)
		{
		// home��
		case VK_HOME:
			xCaret = 0;
			break;

		// end��
		case VK_END:
			xCaret = cxBuffer - 1;
			break;

		// pg up��
		case VK_PRIOR:
			yCaret = 0;
			break;

		// pg dn��
		case VK_NEXT:
			yCaret = cyBuffer - 1;
			break;

		// <-��
		case VK_LEFT:
			xCaret = max(xCaret - 1, 0);
			break;

		// ->��
		case VK_RIGHT:
			xCaret = min(xCaret + 1, cxBuffer - 1);
			break;

		// �ϼ�
		case VK_UP:
			yCaret = max(yCaret - 1, 0);
			break;

		// �¼�
		case VK_DOWN:
			yCaret = min(yCaret + 1, cyBuffer - 1);
			break;

		// del��
		case VK_DELETE:
			// Ҫɾ��ָ��λ�õ�һ���ַ�����Ҫ�Ѻ�����ַ�
			// һ��һ��Ų��ǰ��һ��λ���ϣ��ٽ����һ��λ��
			// ���ַ���Ϊ��
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

	// �����ַ���Ϣ
	case WM_CHAR: // lParam ��ʾ�ظ������� wParam ��ʾ�ַ�����
		for (i = 0; i < (int)LOWORD(lParam); i++) {
			switch (wParam)
			{
			// backspace��
			case '\b':
				if (xCaret > 0) {
					xCaret--;
					SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, 1);
				}
				break;
			
			// tab��
			case '\t':
				do {
					SendMessage(hwnd, WM_CHAR, ' ', 1);
				} while (xCaret % 8 != 0);
				break;

			// enter��
			case '\n':
				if (++yCaret == cyBuffer) {
					yCaret = 0;
				}
				break;

			// enter��
			case '\r':
				xCaret = 0;
				if (++yCaret == cyBuffer) {
					yCaret = 0;
				}
				break;

			// esc��
			case '\x1B': // ʮ�����Ƶ�1B����Ӧ��ASCII�ַ���ESC
				for (y = 0; y < cyBuffer; y++) {
					for (x = 0; x < cxBuffer; x++) {
						BUFFER(x, y) = ' ';
					}
				}

				xCaret = 0;
				yCaret = 0;

				InvalidateRect(hwnd, NULL, FALSE);
				break;

			// ����û����µļ�λ
			default:
				BUFFER(xCaret, yCaret) = (TCHAR)wParam;

				HideCaret(hwnd);
				hdc = GetDC(hwnd);
				SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
				TextOut(hdc, xCaret * cxChar, yCaret * cyChar, &BUFFER(xCaret, yCaret), 1);
				ReleaseDC(hwnd, hdc);
				ShowCaret(hwnd);

				// ���������ˣ�����ת��һ�п�ͷ��ʾ
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

	// ��ʾ�����е���Ϣ��֮ǰ������ÿ��Ϊ��λ��ʾ��
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