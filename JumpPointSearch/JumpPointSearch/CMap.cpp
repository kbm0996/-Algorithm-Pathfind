#include "stdafx.h"

mylib::CMap::CMap(int iWidth, int iHeight)
{
	_iWidth = iWidth;
	_iHeight = iHeight;

	_pMap = (stTILE**)malloc(sizeof(stTILE*)*_iHeight);
	for (int i = 0; i < _iHeight; ++i)
	{
		_pMap[i] = (stTILE*)malloc(sizeof(stTILE)*_iWidth);
		for (int j = 0; j < _iWidth; ++j)
		{
			_pMap[i][j].byType = en_TILE_TYPE_NORMAL;
			_pMap[i][j].bMark = false;
		}
	}
}

mylib::CMap::CMap(const CMap & cpy)
{
	_iWidth = cpy._iWidth;
	_iHeight = cpy._iHeight;

	_pMap = (stTILE**)malloc(sizeof(stTILE*)*_iHeight);
	for (int i = 0; i < _iHeight; ++i)
	{
		_pMap[i] = (stTILE*)malloc(sizeof(stTILE)*_iWidth);
		for (int j = 0; j < _iWidth; ++j)
		{
			_pMap[i][j].byType = cpy._pMap[i]->byType;
			_pMap[i][j].bMark = cpy._pMap[i]->bMark;
			if (_pMap[i]->bMark)
			{
				_pMap[i][j].byR = cpy._pMap[i]->byR;
				_pMap[i][j].byG = cpy._pMap[i]->byG;
				_pMap[i][j].byB = cpy._pMap[i]->byB;
			}
		}
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
		MoveToEx(hdc, x * en_TILE_SIZE_WIDTH, 0, NULL);
		LineTo(hdc, x * en_TILE_SIZE_WIDTH, _iHeight * en_TILE_SIZE_HEIGHT);
	}
	for (int y = 0; y <= _iHeight; ++y)
	{
		MoveToEx(hdc, 0, y * en_TILE_SIZE_HEIGHT, NULL);
		LineTo(hdc, _iWidth * en_TILE_SIZE_WIDTH, y * en_TILE_SIZE_HEIGHT);
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hLinePen);

	// Tile
	HBRUSH hObstacleBrush = CreateSolidBrush(RGB(en_TILE_COLOR_OBS_R, en_TILE_COLOR_OBS_G, en_TILE_COLOR_OBS_B));
	HBRUSH hMarkBrush;
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hObstacleBrush);
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			if (_pMap[iCntH][iCntW].byType == en_TILE_TYPE_OBSTACLE)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, hObstacleBrush);
				Rectangle(hdc, iCntW*en_TILE_SIZE_WIDTH, iCntH*en_TILE_SIZE_HEIGHT, (en_TILE_SIZE_WIDTH + 1) + iCntW*en_TILE_SIZE_WIDTH, (en_TILE_SIZE_HEIGHT + 1) + iCntH*en_TILE_SIZE_HEIGHT);
			}
			else if (_pMap[iCntH][iCntW].bMark)
			{
				hMarkBrush = (HBRUSH)CreateSolidBrush(RGB(_pMap[iCntH][iCntW].byR, _pMap[iCntH][iCntW].byG, _pMap[iCntH][iCntW].byB));
				hOldBrush = (HBRUSH)SelectObject(hdc, hMarkBrush);
				Rectangle(hdc, iCntW*en_TILE_SIZE_WIDTH, iCntH*en_TILE_SIZE_HEIGHT, (en_TILE_SIZE_WIDTH + 1) + iCntW*en_TILE_SIZE_WIDTH, (en_TILE_SIZE_HEIGHT + 1) + iCntH*en_TILE_SIZE_HEIGHT);
				DeleteObject(hMarkBrush);
			}
		}
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(hObstacleBrush);
}

void mylib::CMap::ResetObstacle()
{
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			_pMap[iCntH][iCntW].byType = en_TILE_TYPE_NORMAL;
			_pMap[iCntH][iCntW].bMark = false;
			//_pMap[iCntH][iCntW].byR = en_TILE_COLOR_NORMAL_R;
			//_pMap[iCntH][iCntW].byG = en_TILE_COLOR_NORMAL_G;
			//_pMap[iCntH][iCntW].byB = en_TILE_COLOR_NORMAL_B;
		}
	}
}

void mylib::CMap::ResetMark()
{
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			_pMap[iCntH][iCntW].bMark = false;
			//_pMap[iCntH][iCntW].byR = en_TILE_COLOR_NORMAL_R;
			//_pMap[iCntH][iCntW].byG = en_TILE_COLOR_NORMAL_G;
			//_pMap[iCntH][iCntW].byB = en_TILE_COLOR_NORMAL_B;
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
			rectTmp = { iCntW*en_TILE_SIZE_WIDTH, iCntH*en_TILE_SIZE_HEIGHT, (en_TILE_SIZE_WIDTH + 1) + iCntW*en_TILE_SIZE_WIDTH, (en_TILE_SIZE_HEIGHT + 1) + iCntH*en_TILE_SIZE_HEIGHT };
			if (PtInRect(&rectTmp, mousepos))
			{
				retval = { iCntW, iCntH };
				break;
			}
		}
	}
	return retval;
}

int mylib::CMap::CheckObstacle(POINT mousepos)
{
	for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
	{
		for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
		{
			RECT rect = { iCntW*en_TILE_SIZE_WIDTH, iCntH*en_TILE_SIZE_HEIGHT, (en_TILE_SIZE_WIDTH + 1) + iCntW*en_TILE_SIZE_WIDTH, (en_TILE_SIZE_HEIGHT + 1) + iCntH*en_TILE_SIZE_HEIGHT };
			if (PtInRect(&rect, mousepos))
			{
				if (_pMap[iCntH][iCntW].byType == en_TILE_TYPE_OBSTACLE)
					return en_TILE_TYPE_OBSTACLE;
				else if(_pMap[iCntH][iCntW].byType == en_TILE_TYPE_NORMAL)
					return en_TILE_TYPE_NORMAL;
			}
		}
	}
	return en_TILE_TYPE_OBSTACLE;
}

void mylib::CMap::SetObstacle(POINT mousepos, bool isObstacle)
{
	//WCHAR chTest[20];
	RECT rect;
	if (isObstacle == true)
	{
		for (int iCntH = 0; iCntH < _iHeight; ++iCntH)
		{
			for (int iCntW = 0; iCntW < _iWidth; ++iCntW)
			{
				rect = { iCntW*en_TILE_SIZE_WIDTH, iCntH*en_TILE_SIZE_HEIGHT, (en_TILE_SIZE_WIDTH + 1) + iCntW*en_TILE_SIZE_WIDTH, (en_TILE_SIZE_HEIGHT + 1) + iCntH*en_TILE_SIZE_HEIGHT };
				if (PtInRect(&rect, mousepos))
				{
					//wsprintf(chTest, L"x, y : %d, %d", iCntW, iCntH);
					//SetWindowTextW(g_hWnd, chTest);

					if (_pMap[iCntH][iCntW].byType == en_TILE_TYPE_NORMAL)
					{
						_pMap[iCntH][iCntW].byType = en_TILE_TYPE_OBSTACLE;
						//_pMap[iCntH][iCntW].byR = en_TILE_COLOR_OBS_R;
						//_pMap[iCntH][iCntW].byG = en_TILE_COLOR_OBS_G;
						//_pMap[iCntH][iCntW].byB = en_TILE_COLOR_OBS_B;
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
				rect = { iCntW*en_TILE_SIZE_WIDTH, iCntH*en_TILE_SIZE_HEIGHT, (en_TILE_SIZE_WIDTH + 1) + iCntW*en_TILE_SIZE_WIDTH, (en_TILE_SIZE_HEIGHT + 1) + iCntH*en_TILE_SIZE_HEIGHT };
				if (PtInRect(&rect, mousepos))
				{
					//wsprintf(chTest, L"x, y : %d, %d", iCntW, iCntH);
					//SetWindowTextW(g_hWnd, chTest);

					if (_pMap[iCntH][iCntW].byType == en_TILE_TYPE_OBSTACLE)
					{
						_pMap[iCntH][iCntW].byType = en_TILE_TYPE_NORMAL;
						//_pMap[iCntH][iCntW].byR = en_TILE_COLOR_NORMAL_R;
						//_pMap[iCntH][iCntW].byG = en_TILE_COLOR_NORMAL_G;
						//_pMap[iCntH][iCntW].byB = en_TILE_COLOR_NORMAL_B;
						return;
					}
				}
			}
		}
	}
}