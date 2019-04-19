#include "stdafx.h"

mylib::CAStar::stArea::stArea(int x, int y, stArea * parent)
{
	iX = x;
	iY = y;
	G = 0;
	H = 0;
	F = 0;
	pParent = parent;
}

mylib::CAStar::stArea::stArea(stArea &copy)
{
	iX = copy.iX;
	iY = copy.iY;
	G = copy.G;
	H = copy.H;
	F = copy.F;
	pParent = copy.pParent;
}

mylib::CAStar::CAStar(int iMapWidth, int iMapHeight)
{
	_StartPos.iX = 0;
	_StartPos.iY = 0;
	_DestPos.iX = 0;
	_DestPos.iY = 0;
	_pMap = new CMap(iMapWidth, iMapHeight);

	_isRoute = FALSE;
	_bStart = true;
}

mylib::CAStar::CAStar(CMap& pMap)
{
	_StartPos.iX = 0;
	_StartPos.iY = 0;
	_DestPos.iX = 0;
	_DestPos.iY = 0;
	_pMap = new CMap(pMap);

	_isRoute = FALSE;
	_bStart = true;
}

mylib::CAStar::~CAStar()
{
	delete _pMap;
}

void mylib::CAStar::SetStart(POINT mousepos)
{
	POINT temp = _pMap->GetTilePos(mousepos);
	_StartPos.iX = temp.x;
	_StartPos.iY = temp.y;
}

void mylib::CAStar::SetDest(POINT mousepos)
{
	POINT temp = _pMap->GetTilePos(mousepos);
	_DestPos.iX = temp.x;
	_DestPos.iY = temp.y;
}

bool mylib::CAStar::PathFind()
{
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
		delete *iter;

	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
		delete *iter;

	_Openlst.clear();
	_Closelst.clear();

	_isRoute = FALSE;

	_pStartPos_Temp = new stArea(_StartPos);
	_Openlst.push_back(_pStartPos_Temp);
	while (_Openlst.size() != 0)
	{
		_Openlst.sort(CAStar::compFormula);

		stArea* pCurrent = _Openlst.front();
		if (pCurrent == nullptr)
			break;

		if (SearchOpenlst(_DestPos.iX, _DestPos.iY))
		{
			_isRoute = TRUE;
			break;
		}

		_Closelst.push_back(_Openlst.front());
		_Openlst.pop_front();

		CheckTile_Around(pCurrent);
	}
	return false;
}

void mylib::CAStar::Clear_Process()
{
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
		delete *iter;

	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
		delete *iter;

	_Openlst.clear();
	_Closelst.clear();

	_isRoute = FALSE;
	_bStart = true;
}

bool mylib::CAStar::PathFind_Process()
{
	if (_bStart)
	{
		_pStartPos_Temp = new stArea(_StartPos);
		_Openlst.push_back(_pStartPos_Temp);
		_bStart = false;
		return false;
	}

	_Openlst.sort(CAStar::compFormula);

	stArea* pCurrent = _Openlst.front();
	if (pCurrent == nullptr)	// Fail
		return true;

	if (SearchOpenlst(_DestPos.iX, _DestPos.iY))
	{
		_isRoute = true;		// Success
		return true;
	}

	_Closelst.push_back(_Openlst.front());
	_Openlst.pop_front();

	CheckTile_Around(pCurrent);

	return false;
}

mylib::CMap * mylib::CAStar::GetMap()
{
	return _pMap;
}

void mylib::CAStar::DrawPath(HDC hdc)
{
	// OpenList 
	HBRUSH hBrush = CreateSolidBrush(0xff0000); //blue
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
	{
		Rectangle(hdc, (*iter)->iX*CMap::en_TILE_WIDTH, (*iter)->iY*CMap::en_TILE_HEIGHT, (CMap::en_TILE_WIDTH + 1) + (*iter)->iX*CMap::en_TILE_WIDTH, (CMap::en_TILE_HEIGHT + 1) + (*iter)->iY*CMap::en_TILE_HEIGHT);
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// CloseList
	hBrush = CreateSolidBrush(0x00ffff); //yellow
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
	{
		Rectangle(hdc, (*iter)->iX*CMap::en_TILE_WIDTH, (*iter)->iY*CMap::en_TILE_HEIGHT, (CMap::en_TILE_WIDTH + 1) + (*iter)->iX*CMap::en_TILE_WIDTH, (CMap::en_TILE_HEIGHT + 1) + (*iter)->iY*CMap::en_TILE_HEIGHT);
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// Start
	hBrush = CreateSolidBrush(0x00ff00);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, _StartPos.iX*CMap::en_TILE_WIDTH, _StartPos.iY*CMap::en_TILE_HEIGHT, (CMap::en_TILE_WIDTH + 1) + _StartPos.iX*CMap::en_TILE_WIDTH, (CMap::en_TILE_HEIGHT + 1) + _StartPos.iY*CMap::en_TILE_HEIGHT);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// Dest
	hBrush = CreateSolidBrush(0x0000ff);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, _DestPos.iX*CMap::en_TILE_WIDTH, _DestPos.iY*CMap::en_TILE_HEIGHT, (CMap::en_TILE_WIDTH + 1) + _DestPos.iX*CMap::en_TILE_WIDTH, (CMap::en_TILE_HEIGHT + 1) + _DestPos.iY*CMap::en_TILE_HEIGHT);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// Start to Dest
	HPEN hPen = CreatePen(PS_SOLID, 3, 0x0000ff);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	if (_isRoute)
	{
		stArea* pTemp = _Openlst.front();// peek_front();
		while(pTemp->pParent != nullptr)
		{
			MoveToEx(hdc, (pTemp->iX) * CMap::en_TILE_WIDTH + CMap::en_TILE_WIDTH / 2, (pTemp->iY) * CMap::en_TILE_HEIGHT + CMap::en_TILE_HEIGHT / 2, NULL);
			LineTo(hdc, ((pTemp->pParent)->iX) * CMap::en_TILE_WIDTH + CMap::en_TILE_WIDTH / 2, ((pTemp->pParent)->iY) * CMap::en_TILE_HEIGHT + CMap::en_TILE_HEIGHT / 2);
			pTemp = pTemp->pParent;
		}
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

mylib::CAStar::stArea * mylib::CAStar::SearchOpenlst(int iX, int iY)
{
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
	{
		if ((*iter)->iX == iX && (*iter)->iY == iY)
			return *iter;
	}
	return nullptr;
}

mylib::CAStar::stArea * mylib::CAStar::SearchCloselst(int iX, int iY)
{
	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
	{
		if ((*iter)->iX == iX && (*iter)->iY == iY)
			return *iter;
	}
	return nullptr;
}


BOOL mylib::CAStar::CheckTile(int iX, int iY)
{
	// Check Range
	if (iX < 0 || iX >= _pMap->_iWidth || 
		iY < 0 || iY >= _pMap->_iHeight)
		return FALSE;

	// Check Obstacle
	if (_pMap->_pMap[iY][iX] == FALSE)
		return FALSE;

	return TRUE;
}

BOOL mylib::CAStar::CheckTile_Around(stArea * current)
{
	BOOL retval = FALSE;
	stArea* pTemp;

	// 8
	if (CheckTile(current->iX, current->iY + 1) && !SearchCloselst(current->iX, current->iY + 1))
	{
		pTemp = SearchOpenlst(current->iX, current->iY + 1);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX, current->iY + 1, current);
			pTemp->G = current->G + 10;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 10)
			{
				pTemp->G = current->G + 10;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}

	// 6
	if (CheckTile(current->iX + 1, current->iY) && !SearchCloselst(current->iX + 1, current->iY))
	{
		pTemp = SearchOpenlst(current->iX + 1, current->iY);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX + 1, current->iY, current);
			pTemp->G = current->G + 10;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 10)
			{
				pTemp->G = current->G + 10;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}

	// 2
	if (CheckTile(current->iX, current->iY - 1) && !SearchCloselst(current->iX, current->iY - 1))
	{
		pTemp = SearchOpenlst(current->iX, current->iY - 1);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX, current->iY - 1, current);
			pTemp->G = current->G + 10;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 10)
			{
				pTemp->G = current->G + 10;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}

	// 4
	if (CheckTile(current->iX - 1, current->iY) && !SearchCloselst(current->iX - 1, current->iY))
	{
		pTemp = SearchOpenlst(current->iX - 1, current->iY);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX - 1, current->iY, current);
			pTemp->G = current->G + 10;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 10)
			{
				pTemp->G = current->G + 10;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}
	// 9 /
	if (CheckTile(current->iX + 1, current->iY + 1) && !SearchCloselst(current->iX + 1, current->iY + 1))
	{
		pTemp = SearchOpenlst(current->iX + 1, current->iY + 1);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX + 1, current->iY + 1, current);
			pTemp->G = current->G + 14;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 14)
			{
				pTemp->G = current->G + 14;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}

	// 3 /
	if (CheckTile(current->iX + 1, current->iY - 1) && !SearchCloselst(current->iX + 1, current->iY - 1))
	{
		pTemp = SearchOpenlst(current->iX + 1, current->iY - 1);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX + 1, current->iY - 1, current);
			pTemp->G = current->G + 14;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 14)
			{
				pTemp->G = current->G + 14;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}
	// 1 /
	if (CheckTile(current->iX - 1, current->iY - 1) && !SearchCloselst(current->iX - 1, current->iY - 1))
	{
		pTemp = SearchOpenlst(current->iX - 1, current->iY - 1);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX - 1, current->iY - 1, current);
			pTemp->G = current->G + 14;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 14)
			{
				pTemp->G = current->G + 14;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}

	// 7 /
	if (CheckTile(current->iX - 1, current->iY + 1) && !SearchCloselst(current->iX - 1, current->iY + 1))
	{
		pTemp = SearchOpenlst(current->iX - 1, current->iY + 1);
		if (!pTemp)
		{
			pTemp = new stArea(current->iX - 1, current->iY + 1, current);
			pTemp->G = current->G + 14;
			pTemp->H = GetDistance(pTemp, &_DestPos);
			pTemp->F = pTemp->G + pTemp->H;
			_Openlst.push_front(pTemp);
			retval = TRUE;
		}
		else
		{
			if (pTemp->G > current->G + 14)
			{
				pTemp->G = current->G + 14;
				//pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				pTemp->pParent = current;
				retval = TRUE;
			}
		}
	}
	return retval;
}

float mylib::CAStar::GetDistance(const stArea * from, const stArea * to)
{
	return 10 * (abs(from->iX - to->iX) + abs(from->iY - to->iY));
}

bool mylib::CAStar::compFormula(const stArea * a, const stArea * b)
{
	return a->F < b->F;
}
