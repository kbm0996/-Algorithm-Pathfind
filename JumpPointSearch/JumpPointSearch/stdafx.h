// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#include <list>
#include "CBresenham.h"
#include "CMap.h"
#include "CAStar.h"

extern HWND		g_hWnd;
extern RECT		g_crt;
extern HDC		g_hMemDC;
extern HBITMAP	g_hMemBitmap;
extern HBITMAP	g_hMemBitmapOld;