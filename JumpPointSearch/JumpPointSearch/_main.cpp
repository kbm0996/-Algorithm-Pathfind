#include "stdafx.h"
#include "JumpPointSearch.h"

#define __PROCESS_RENDER_ // Annotation -> Exclude rendering process

mylib::CAStar g_AStar;
bool g_bCheckObstacle;

HWND	g_hWnd;
RECT	g_crt;
HDC		g_hMemDC;
HBITMAP g_hMemBitmap;
HBITMAP g_hMemBitmapOld;

__int64 GetQPCTick();

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	timeBeginPeriod(1);
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPPOINTSEARCH));

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			PatBlt(g_hMemDC, 0, 0, g_crt.right, g_crt.bottom, WHITENESS);

			g_AStar.GetMap()->DrawMap(g_hMemDC);
			g_AStar.DrawPath(g_hMemDC);

			InvalidateRect(g_hWnd, NULL, false);
		}
	}
	timeEndPeriod(1);
	return (int)msg.wParam;
}

__int64 GetQPCTick()//LARGE_INTEGER& li)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return static_cast<__int64>(li.QuadPart);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JUMPPOINTSEARCH));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_JUMPPOINTSEARCH);
	wcex.lpszClassName = L"A*";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hWnd = CreateWindowW(L"A*", L"A*", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LARGE_INTEGER li;
	static __int64 StartTick;
	static __int64 EndTick;

	static HANDLE hTimer;
	static bool bTimer;

	POINT MousePos;

	switch (message)
	{
	case WM_CREATE:
	{
		// Init QueryPerformanceCounter
		QueryPerformanceFrequency(&li);

		// Init Rendering
		GetClientRect(hWnd, &g_crt);

		HDC hdc = GetDC(hWnd);
		g_hMemDC = CreateCompatibleDC(hdc);
		g_hMemBitmap = CreateCompatibleBitmap(hdc, g_crt.right, g_crt.bottom);
		g_hMemBitmapOld = (HBITMAP)SelectObject(g_hMemDC, g_hMemBitmap);
		ReleaseDC(hWnd, hdc);

		PatBlt(g_hMemDC, 0, 0, g_crt.right, g_crt.bottom, WHITENESS);
		break;
	}
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
		{
			MousePos.x = GET_X_LPARAM(lParam);
			MousePos.y = GET_Y_LPARAM(lParam);
			g_AStar.GetMap()->SetObstacle(MousePos, g_bCheckObstacle);
		}
		break;
	case WM_LBUTTONDOWN:
		MousePos.x = GET_X_LPARAM(lParam);
		MousePos.y = GET_Y_LPARAM(lParam);
		g_bCheckObstacle = g_AStar.GetMap()->CheckObstacle(MousePos);
		break;
	case WM_LBUTTONDBLCLK:
		MousePos.x = GET_X_LPARAM(lParam);
		MousePos.y = GET_Y_LPARAM(lParam);
		g_AStar.SetStart(MousePos);
		break;
	case WM_RBUTTONDOWN:
	{
		StartTick = GetQPCTick();// li);
#ifndef __PROCESS_RENDER_
								 // No Process
		g_AStar.PathFind();
		EndTick = GetQPCTick();// li);

		WCHAR szContent[100] = { 0, };
		swprintf_s(szContent, L"Consumed time : %8.6f s", (float)((EndTick - StartTick) / (float)li.QuadPart));
		MessageBox(hWnd, szContent, L"Result", MB_OK | MB_ICONINFORMATION);
#else
								 // Process
		g_AStar.Clear_Process();
		hTimer = (HANDLE)SetTimer(hWnd, 1, USER_TIMER_MINIMUM, NULL); // Set Timer(ID:1)
		SendMessage(hWnd, WM_TIMER, 1, 0); // Activate Timer(ID:1)
		bTimer = true;
#endif

		break;
	}
	case WM_TIMER:
		if (bTimer)
		{
			if (g_AStar.PathFind_Process())
			{
				bTimer = false;
				KillTimer(hWnd, 1);	// Off Timer(ID:1)
				EndTick = GetQPCTick();

				WCHAR szContent[100] = { 0, };
				swprintf_s(szContent, L"Consumed time : %8.6f s", (float)((EndTick - StartTick) / (float)li.QuadPart));
				MessageBox(hWnd, szContent, L"Result", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
	case WM_RBUTTONDBLCLK:
		MousePos.x = GET_X_LPARAM(lParam);
		MousePos.y = GET_Y_LPARAM(lParam);
		g_AStar.SetDest(MousePos);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_RESET:
			g_AStar.GetMap()->ResetObstacle();
			break;
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, g_crt.right, g_crt.bottom, g_hMemDC, 0, 0, SRCCOPY);

		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		SelectObject(g_hMemDC, g_hMemBitmapOld);
		DeleteObject(g_hMemBitmap);
		DeleteDC(g_hMemDC);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}