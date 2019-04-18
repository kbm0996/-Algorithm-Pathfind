#include "stdafx.h"

mylib::CMap::CMap(int iWidth, int iHeight)
{
	_iWidth = iWidth;
	_iHeight = iHeight;

	_pMap = (char**)malloc(sizeof(char**)*iHeight);
	memset(_pMap, TRUE, sizeof(char*)*iHeight);
	for (int i = 0; i < iHeight; ++i)
	{
		_pMap[i] = (char*)malloc(sizeof(char*)*iWidth);
		memset(_pMap[i], TRUE, sizeof(char*)*iWidth);
	}
}

mylib::CMap::CMap(const CMap & cpy)
{
	_iWidth = cpy._iWidth;
	_iHeight = cpy._iHeight;

	_pMap = (char**)malloc(sizeof(char**)*_iHeight);
	memcpy_s(_pMap, sizeof(char*)*_iHeight, cpy._pMap, sizeof(char*)*_iHeight);
	for (int i = 0; i < _iHeight; ++i)
	{
		_pMap[i] = (char*)malloc(sizeof(char*)*_iWidth);
		memcpy_s(_pMap[i], sizeof(char*)*_iWidth, cpy._pMap[i], sizeof(char*)*_iWidth);
	}
}

mylib::CMap::~CMap()
{
	for (int i = 0; i < _iHeight; ++i)
		free(_pMap[i]);
	free(_pMap);
}

void mylib::CMap::DrawMap(HDC hdc)
{
	// Horizontal Line & Vertical Line
	HPEN hLinePen = CreatePen(PS_SOLID, 1, 0x707070);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hLinePen);
	for (int x = 0; x <= _iWidth; ++x)
	{
		MoveToEx(hdc, x * en_TILE_WIDTH, 0, NULL);
		LineTo(hdc, x * en_TILE_WIDTH, _iHeight * en_TILE_HEIGHT);
	}
	for (int y = 0; y <= _iHeight; ++y)
	{
		MoveToEx(hdc, 0, y * en_TILE_HEIGHT, NULL);
		LineTo(hdc, _iWidth * en_TILE_WIDTH, y * en_TILE_HEIGHT);
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hLinePen);

	// Obstacle
	HBRUSH hBrush = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			if (_pMap[iCntH][iCntW] == FALSE)
			{
				Rectangle(hdc, iCntW*en_TILE_WIDTH, iCntH*en_TILE_HEIGHT, (en_TILE_WIDTH + 1) + iCntW*en_TILE_WIDTH, (en_TILE_HEIGHT + 1) + iCntH*en_TILE_HEIGHT);
			}
		}
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}

void mylib::CMap::ResetObstacle()
{
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			_pMap[iCntH][iCntW] = TRUE;
		}
	}
}

POINT mylib::CMap::GetTilePos(POINT mousepos)
{
	RECT rectTmp;
	POINT retval = { 0,0 };
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			rectTmp = { iCntW*en_TILE_WIDTH, iCntH*en_TILE_HEIGHT, (en_TILE_WIDTH + 1) + iCntW*en_TILE_WIDTH, (en_TILE_HEIGHT + 1) + iCntH*en_TILE_HEIGHT };
			if (PtInRect(&rectTmp, mousepos))
			{
				retval = { iCntW, iCntH };
				break;
			}
		}
	}
	return retval;
}

bool mylib::CMap::CheckObstacle(POINT mousepos)
{
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			RECT rect = { iCntW*en_TILE_WIDTH, iCntH*en_TILE_HEIGHT, (en_TILE_WIDTH + 1) + iCntW*en_TILE_WIDTH, (en_TILE_HEIGHT + 1) + iCntH*en_TILE_HEIGHT };
			if (PtInRect(&rect, mousepos))
			{
				if (_pMap[iCntH][iCntW] == FALSE)
					return false;
				else
					return true;
			}
		}
	}
	return true;
}

void mylib::CMap::SetObstacle(POINT mousepos, bool isObstacle)
{
	WCHAR chTest[20];
	RECT rect;
	if (isObstacle == true)
	{
		for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
		{
			for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
			{
				rect = { iCntW*en_TILE_WIDTH, iCntH*en_TILE_HEIGHT, (en_TILE_WIDTH + 1) + iCntW*en_TILE_WIDTH, (en_TILE_HEIGHT + 1) + iCntH*en_TILE_HEIGHT };
				if (PtInRect(&rect, mousepos))
				{
					wsprintf(chTest, L"x, y : %d, %d", iCntW, iCntH);
					SetWindowTextW(g_hWnd, chTest);

					if (_pMap[iCntH][iCntW] == TRUE)
					{
						_pMap[iCntH][iCntW] = FALSE;
						return;
					}
				}
			}
		}
	}
	else
	{
		for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
		{
			for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
			{
				rect = { iCntW*en_TILE_WIDTH, iCntH*en_TILE_HEIGHT, (en_TILE_WIDTH + 1) + iCntW*en_TILE_WIDTH, (en_TILE_HEIGHT + 1) + iCntH*en_TILE_HEIGHT };
				if (PtInRect(&rect, mousepos))
				{
					wsprintf(chTest, L"x, y : %d, %d", iCntW, iCntH);
					SetWindowTextW(g_hWnd, chTest);

					if (_pMap[iCntH][iCntW] == FALSE)
					{
						_pMap[iCntH][iCntW] = TRUE;
						return;
					}
				}
			}
		}
	}
}