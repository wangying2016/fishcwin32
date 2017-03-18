/* -------------------------------------------------------------------
MyWindows.c -- ��������ģ��
��Windows ������ƣ�SDK������Ƶ�̳�
--------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>
#include <strsafe.h>

#define PI 3.1415926                       //Բ����
//#define R(theta)  (exp(sin(theta)) + 2 * cos(theta) + pow(sin(49 * theta), 4))      //����ͼ�������޸Ĵ˴������Եõ���ͬ�ĺ���ͼ��
//#define R(theta) (3 * sin(theta) + 3.5 * cos(10 * theta) * cos(8 * theta))
#define R(theta) (0.2 * sin(3 * theta) + sin(4 * theta) + 2 * sin(5 * theta) + 1.9 * sin(7 * theta) - 0.2 * sin(9 * theta) + sin(11 * theta))
#define P_MIN (0)                            //������˵� 
#define P_MAX (2 * PI)                    //�����Ҷ˵㡾�޸Ĵ˴����ϴ������Եõ���ͬ����ͼ���䡿
#define LIMIT 5                             //����������� ���޸Ĵ˴������Զ�ͼ��������š�
#define S_NUM 720                         //������

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
	static int cxClient, cyClient;
	POINT apt[S_NUM];
	double theta;
	int x, y, cxCenter, cyCenter;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		cxCenter = cxClient / 2;
		cyCenter = cyClient / 2; 
		// ��������
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HGDIOBJ hOldPen = SelectObject(hdc, hPen);
		// ����ͼ��
		x = (int)(R(P_MIN) * cos(P_MIN) * cxCenter / LIMIT + cxCenter);
		y = (int)(-1 * R(P_MIN) * sin(P_MIN) * cyCenter / LIMIT + cyCenter);   //�����ʼ��Ļ����
		MoveToEx(hdc, x, y, NULL);

		for (int i = 0; i < S_NUM; i++)
		{
			theta = (double)((P_MAX - P_MIN) * i / S_NUM + P_MIN);  //����Ƕ�

			apt[i].x = (int)(R(theta) * cos(theta) * cxCenter / LIMIT + cxCenter);
			apt[i].y = (int)(-1 * R(theta) * sin(theta) * cyCenter / LIMIT + cyCenter);  //������Ļ����
		}

		PolylineTo(hdc, apt, S_NUM);
		// �ָ�ԭ���Ļ���
		SelectObject(hdc, hOldPen);
		// ɾ�������Ļ���
		DeleteObject(hPen);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}