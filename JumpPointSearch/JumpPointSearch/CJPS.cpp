#include "stdafx.h"


mylib::CJPS::stArea::stArea(int x, int y, stArea * parent, stArea * parentbresenham)
{
	iX = x;
	iY = y;
	G = 0;
	H = 0;
	F = 0;
	pParent = parent;
	pParent_Bresenham = parentbresenham;
}

mylib::CJPS::stArea::stArea(stArea &copy)
{
	iX = copy.iX;
	iY = copy.iY;
	G = copy.G;
	H = copy.H;
	F = copy.F;
	pParent = copy.pParent;
	pParent_Bresenham = copy.pParent_Bresenham;
}

mylib::CJPS::CJPS(int iMapWidth, int iMapHeight)
{
	_StartPos.iX = 0;
	_StartPos.iY = 0;
	_DestPos.iX = 1;
	_DestPos.iY = 1;
	_pCMap = new CMap(iMapWidth, iMapHeight);

	_isRoute = FALSE;
	_bStart = true;
}

mylib::CJPS::CJPS(CMap& pMap)
{
	_StartPos.iX = 0;
	_StartPos.iY = 0;
	_DestPos.iX = 1;
	_DestPos.iY = 1;
	_pCMap = new CMap(pMap);
	_isRoute = FALSE;
	_bStart = true;
	_DestPos_Mark = _DestPos;
}

mylib::CJPS::~CJPS()
{
	delete _pCMap;
}

void mylib::CJPS::SetStart(POINT mousepos)
{
	POINT temp = _pCMap->GetTilePos(mousepos);
	_StartPos.iX = temp.x;
	_StartPos.iY = temp.y;
}

void mylib::CJPS::SetDest(POINT mousepos)
{
	POINT temp = _pCMap->GetTilePos(mousepos);
	_DestPos.iX = temp.x;
	_DestPos.iY = temp.y;
}

void mylib::CJPS::GetBresenhamPath(stArea * pDestNode)
{
	stArea * pNode = pDestNode;
	while (pNode->pParent != nullptr)
	{
		pNode->pParent_Bresenham = pNode->pParent;
		pNode = pNode->pParent;
	}

	
	pNode = pDestNode;
	while (1)
	{
		if (pNode == nullptr)
			break;

		if (pNode->pParent_Bresenham != nullptr)
		{

			stArea * pNode_Parent = pNode->pParent_Bresenham;
			while (1)
			{
				if (pNode_Parent == nullptr)
					break;

				int iCurX, iCurY;
				CBresenham Bre(pNode->iX, pNode->iY, pNode_Parent->iX, pNode_Parent->iY);
				while (!Bre.PeekNext(&iCurX, &iCurY))
				{
					if (!isWalkable(iCurX, iCurY))
						break;

					Bre.GetNext(&iCurX, &iCurY);
				}

				if (iCurX == pNode_Parent->iX && iCurY == pNode_Parent->iY)
				{
					pNode->pParent_Bresenham = pNode_Parent;
					pNode_Parent = pNode_Parent->pParent_Bresenham;
				}
				else
					break;
			}
		}
		pNode = pNode->pParent_Bresenham;
	}
}

bool mylib::CJPS::PathFind(HDC hdc, bool bBresenham, int iCheckRangeCnt, bool bDomainMark)
{
	_pCMap->ResetMark();
	Clear();
	_pStartPos_Temp = new stArea(_StartPos);
	_Openlst.push_back(_pStartPos_Temp);
	while (_Openlst.size() != 0)
	{
		_Openlst.sort(CJPS::compFormula);
		stArea* pCurrent = _Openlst.front();
		if (pCurrent == nullptr)
			break;

		stArea * pDest = SearchOpenlst(_DestPos.iX, _DestPos.iY);
		if (pDest != nullptr)
		{
			_isRoute = TRUE;
			_DestPos_Mark.pParent = pDest->pParent;
			if(bBresenham)
				GetBresenhamPath(&_DestPos_Mark);
			return true;
		}

		_Closelst.push_back(_Openlst.front());
		_Openlst.pop_front();

		if(bDomainMark)
			RandColor_JumpDomain();
		
		CheckTile_Around(pCurrent);
	}
	return false;
}

bool mylib::CJPS::PathFind_Process(HDC hdc, bool bBresenham, int iCheckRangeCnt, bool bDomainMark)
{
	if (_bStart)
	{
		_pCMap->ResetMark();
		_pStartPos_Temp = new stArea(_StartPos);
		_Openlst.push_back(_pStartPos_Temp);
		_bStart = false;
		return false;
	}

	_Openlst.sort(CJPS::compFormula);
	stArea* pCurrent = _Openlst.front();
	if (pCurrent == nullptr)	// Fail
		return true;

	stArea * pDest = SearchOpenlst(_DestPos.iX, _DestPos.iY);
	if (pDest != nullptr)
	{
		_isRoute = TRUE;	// Success
		_DestPos_Mark.pParent = pDest->pParent;
		if (bBresenham)
			GetBresenhamPath(&_DestPos_Mark);
		return true;
	}

	_Closelst.push_back(_Openlst.front());
	_Openlst.pop_front();

	if(bDomainMark)
		RandColor_JumpDomain();

	CheckTile_Around(pCurrent);
	return false;
}

void mylib::CJPS::DrawPath(HDC hdc)
{
	// OpenList 
	HBRUSH hBrush = CreateSolidBrush(0xff0000); //blue
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
	{
		Rectangle(hdc, (*iter)->iX*CMap::en_TILE_SIZE_WIDTH, (*iter)->iY*CMap::en_TILE_SIZE_HEIGHT, (CMap::en_TILE_SIZE_WIDTH + 1) + (*iter)->iX*CMap::en_TILE_SIZE_WIDTH, (CMap::en_TILE_SIZE_HEIGHT + 1) + (*iter)->iY*CMap::en_TILE_SIZE_HEIGHT);
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// CloseList
	hBrush = CreateSolidBrush(0x00ffff); //yellow
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
	{
		Rectangle(hdc, (*iter)->iX*CMap::en_TILE_SIZE_WIDTH, (*iter)->iY*CMap::en_TILE_SIZE_HEIGHT, (CMap::en_TILE_SIZE_WIDTH + 1) + (*iter)->iX*CMap::en_TILE_SIZE_WIDTH, (CMap::en_TILE_SIZE_HEIGHT + 1) + (*iter)->iY*CMap::en_TILE_SIZE_HEIGHT);
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// Start
	hBrush = CreateSolidBrush(0x00ff00);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, _StartPos.iX*CMap::en_TILE_SIZE_WIDTH, _StartPos.iY*CMap::en_TILE_SIZE_HEIGHT, (CMap::en_TILE_SIZE_WIDTH + 1) + _StartPos.iX*CMap::en_TILE_SIZE_WIDTH, (CMap::en_TILE_SIZE_HEIGHT + 1) + _StartPos.iY*CMap::en_TILE_SIZE_HEIGHT);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// Dest
	hBrush = CreateSolidBrush(0x0000ff);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, _DestPos.iX*CMap::en_TILE_SIZE_WIDTH, _DestPos.iY*CMap::en_TILE_SIZE_HEIGHT, (CMap::en_TILE_SIZE_WIDTH + 1) + _DestPos.iX*CMap::en_TILE_SIZE_WIDTH, (CMap::en_TILE_SIZE_HEIGHT + 1) + _DestPos.iY*CMap::en_TILE_SIZE_HEIGHT);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	// Start to Dest
	HPEN hPen = CreatePen(PS_SOLID, 3, 0x0000ff);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	if (_isRoute)
	{
		stArea* pTemp = &_DestPos_Mark;//_Openlst.front();
		while (pTemp->pParent != nullptr)
		{
			MoveToEx(hdc, (pTemp->iX) * CMap::en_TILE_SIZE_WIDTH + CMap::en_TILE_SIZE_WIDTH / 2, (pTemp->iY) * CMap::en_TILE_SIZE_HEIGHT + CMap::en_TILE_SIZE_HEIGHT / 2, NULL);
			LineTo(hdc, ((pTemp->pParent)->iX) * CMap::en_TILE_SIZE_WIDTH + CMap::en_TILE_SIZE_WIDTH / 2, ((pTemp->pParent)->iY) * CMap::en_TILE_SIZE_HEIGHT + CMap::en_TILE_SIZE_HEIGHT / 2);
			pTemp = pTemp->pParent;
		}
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);


	// Start to Dest(Bresenham)
	hPen = CreatePen(PS_SOLID, 2, 0x000000);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	if (_isRoute)
	{
		stArea* pTemp = &_DestPos_Mark;//_Openlst.front();
		while (pTemp->pParent_Bresenham != nullptr)
		{
			MoveToEx(hdc, (pTemp->iX) * CMap::en_TILE_SIZE_WIDTH + CMap::en_TILE_SIZE_WIDTH / 2, (pTemp->iY) * CMap::en_TILE_SIZE_HEIGHT + CMap::en_TILE_SIZE_HEIGHT / 2, NULL);
			LineTo(hdc, ((pTemp->pParent_Bresenham)->iX) * CMap::en_TILE_SIZE_WIDTH + CMap::en_TILE_SIZE_WIDTH / 2, ((pTemp->pParent_Bresenham)->iY) * CMap::en_TILE_SIZE_HEIGHT + CMap::en_TILE_SIZE_HEIGHT / 2);
			pTemp = pTemp->pParent_Bresenham;
		}
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void mylib::CJPS::RandColor_JumpDomain()
{
	_byR = rand() % 20 + 230;
	_byG = rand() % 20 + 230;
	_byB = rand() % 20 + 230;
}

mylib::CJPS::stArea * mylib::CJPS::SearchOpenlst(int iX, int iY)
{
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
	{
		if ((*iter)->iX == iX && (*iter)->iY == iY)
			return *iter;
	}
	return nullptr;
}

mylib::CJPS::stArea * mylib::CJPS::SearchCloselst(int iX, int iY)
{
	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
	{
		if ((*iter)->iX == iX && (*iter)->iY == iY)
			return *iter;
	}
	return nullptr;
}

void mylib::CJPS::Clear()
{
	for (auto iter = _Openlst.begin(); iter != _Openlst.end(); ++iter)
		delete *iter;

	for (auto iter = _Closelst.begin(); iter != _Closelst.end(); ++iter)
		delete *iter;

	_Openlst.clear();
	_Closelst.clear();

	_isRoute = FALSE;
	_bStart = true;
	_DestPos_Mark = _DestPos;
}

BOOL mylib::CJPS::Jump(int iX, int iY, en_DIR_JUMP enDir, int * pOutX, int * pOutY)
{
	while (1)
	{
		if (!isWalkable(iX, iY))
			return FALSE;

		_pCMap->_pMap[iY][iX].bMark = true;
		_pCMap->_pMap[iY][iX].byR = _byR;
		_pCMap->_pMap[iY][iX].byG = _byG;
		_pCMap->_pMap[iY][iX].byB = _byB;


		if (iX == _DestPos.iX && iY == _DestPos.iY)
		{
			*pOutX = iX;
			*pOutY = iY;
			return TRUE;
		}

		switch (enDir)
		{
		case en_DIR_UU:
			if ((!isWalkable(iX - 1, iY) && isWalkable(iX - 1, iY - 1)) ||
				(!isWalkable(iX + 1, iY) && isWalkable(iX + 1, iY - 1)))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			--iY;
			break;
		case en_DIR_DD:
			if ((!isWalkable(iX - 1, iY) && isWalkable(iX - 1, iY + 1)) ||
				(!isWalkable(iX + 1, iY) && isWalkable(iX + 1, iY + 1)))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			++iY;
			break;
		case en_DIR_LL:
			if ((!isWalkable(iX, iY - 1) && isWalkable(iX - 1, iY - 1)) ||
				(!isWalkable(iX, iY + 1) && isWalkable(iX - 1, iY + 1)))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			--iX;
			break;
		case en_DIR_RR:
			if ((!isWalkable(iX, iY - 1) && isWalkable(iX + 1, iY - 1)) ||
				(!isWalkable(iX, iY + 1) && isWalkable(iX + 1, iY + 1)))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			++iX;
			break;
		case en_DIR_UR:
			if ((!isWalkable(iX - 1, iY) && isWalkable(iX - 1, iY - 1)) ||
				(!isWalkable(iX, iY + 1) && isWalkable(iX + 1, iY + 1)))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}

			if (Jump(iX, iY - 1, en_DIR_UU, pOutX, pOutY) ||
				Jump(iX + 1, iY, en_DIR_RR, pOutX, pOutY))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			++iX;
			--iY;
			break;
		case en_DIR_DR:
			if ((!isWalkable(iX - 1, iY) && isWalkable(iX - 1, iY + 1)) ||
				(!isWalkable(iX, iY - 1) && isWalkable(iX + 1, iY - 1))) {
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}

			if (Jump(iX, iY + 1, en_DIR_DD, pOutX, pOutY) ||
				Jump(iX + 1, iY, en_DIR_RR, pOutX, pOutY))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			++iX;
			++iY;
			break;
		case en_DIR_DL:
			if ((!isWalkable(iX, iY - 1) && isWalkable(iX - 1, iY - 1)) ||
				(!isWalkable(iX + 1, iY) && isWalkable(iX + 1, iY + 1))) {
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			if (Jump(iX, iY + 1, en_DIR_DD, pOutX, pOutY) ||
				Jump(iX - 1, iY, en_DIR_LL, pOutX, pOutY))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			--iX;
			++iY;
			break;
		case en_DIR_UL:
			if ((!isWalkable(iX + 1, iY) && isWalkable(iX + 1, iY - 1)) ||
				(!isWalkable(iX, iY + 1) && isWalkable(iX - 1, iY + 1))) {
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			if (Jump(iX, iY - 1, en_DIR_UU, pOutX, pOutY) ||
				Jump(iX - 1, iY, en_DIR_LL, pOutX, pOutY))
			{
				*pOutX = iX;
				*pOutY = iY;
				return TRUE;
			}
			--iX;
			--iY;
			break;
		}
	}
	return FALSE;
}

BOOL mylib::CJPS::CheckTile_Around(stArea * current)
{
	stArea* pTemp;
	int iJumpX = 0;
	int iJumpY = 0;

	// * Start Case :: If the current node is the starting node
	if (current->pParent == nullptr)
	{
		if (Jump(current->iX, current->iY - 1, en_DIR_UU, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX, current->iY - 1))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 10)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX, current->iY + 1, en_DIR_DD, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX, current->iY + 1))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 10)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX - 1, current->iY, en_DIR_LL, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX - 1, current->iY))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 10)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX + 1, current->iY, en_DIR_RR, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX + 1, current->iY))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 10)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX + 1, current->iY - 1, en_DIR_UR, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX + 1, current->iY - 1))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 14)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX + 1, current->iY + 1, en_DIR_DR, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX + 1, current->iY + 1))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 14)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX - 1, current->iY + 1, en_DIR_DL, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX - 1, current->iY + 1))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 14)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		if (Jump(current->iX - 1, current->iY - 1, en_DIR_UL, &iJumpX, &iJumpY) &&
			!SearchCloselst(current->iX - 1, current->iY - 1))
		{
			pTemp = SearchOpenlst(iJumpX, iJumpY);
			if (!pTemp)
			{
				pTemp = new stArea(iJumpX, iJumpY, current);
				pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
				pTemp->H = GetDistance(pTemp, &_DestPos);
				pTemp->F = pTemp->G + pTemp->H;
				_Openlst.push_front(pTemp);
			}
			else
			{
				if (pTemp->G > current->G + 14)
				{
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->F = pTemp->G + pTemp->H;
					pTemp->pParent = current;
				}
			}
		}
		return TRUE;
	}


	// * Normal Case
	// x - 1y - 1	xy - 1		x + 1y - 1
	// x - 1y		xy			x + 1y
	// x - 1y + 1	xy + 1		x + 1y + 1
	// parent - base
	if ((current->pParent)->iX == current->iX)
	{
		// UU
		if ((current->pParent)->iY - current->iY > 0)	// 부모가 DD 방향이면 
		{
			if (Jump(current->iX, current->iY - 1, en_DIR_UU, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX, current->iY - 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
				if (Jump(current->iX, current->iY - 1, en_DIR_UU, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX, current->iY))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 10)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			}
			if (!isWalkable(current->iX - 1, current->iY) && isWalkable(current->iX - 1, current->iY - 1))
				if (Jump(current->iX - 1, current->iY - 1, en_DIR_UL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			if (!isWalkable(current->iX + 1, current->iY) && isWalkable(current->iX + 1, current->iY - 1))
				if (Jump(current->iX + 1, current->iY - 1, en_DIR_UR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			return TRUE;
		}
		// DD
		if ((current->pParent)->iY - current->iY < 0)
		{
			if (Jump(current->iX, current->iY + 1, en_DIR_DD, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX, current->iY + 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (!isWalkable(current->iX - 1, current->iY) && isWalkable(current->iX - 1, current->iY + 1))
				if (Jump(current->iX - 1, current->iY + 1, en_DIR_DL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			if (!isWalkable(current->iX + 1, current->iY) && isWalkable(current->iX + 1, current->iY + 1))
				if (Jump(current->iX + 1, current->iY + 1, en_DIR_DR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			return TRUE;
		}
	}
	if ((current->pParent)->iX - current->iX < 0)
	{
		// RR
		if ((current->pParent)->iY - current->iY == 0)	// 부모가 LL 방향이면 
		{
			if (Jump(current->iX + 1, current->iY, en_DIR_RR, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX + 1, current->iY))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (!isWalkable(current->iX, current->iY - 1) && isWalkable(current->iX + 1, current->iY - 1))
				if (Jump(current->iX + 1, current->iY - 1, en_DIR_UR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			if (!isWalkable(current->iX, current->iY + 1) && isWalkable(current->iX + 1, current->iY + 1))
				if (Jump(current->iX + 1, current->iY + 1, en_DIR_DR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			return TRUE;
		}
		// UR
		if ((current->pParent)->iY - current->iY > 0)	// 부모가 DL 방향
		{
			if (Jump(current->iX + 1, current->iY - 1, en_DIR_UR, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX + 1, current->iY - 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 14)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX, current->iY - 1, en_DIR_UU, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX, current->iY - 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX + 1, current->iY, en_DIR_RR, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX + 1, current->iY))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (!isWalkable(current->iX - 1, current->iY) && isWalkable(current->iX - 1, current->iY - 1))
			{
				if (Jump(current->iX - 1, current->iY - 1, en_DIR_UL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14; 
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			}
			if (!isWalkable(current->iX, current->iY + 1) && isWalkable(current->iX + 1, current->iY + 1))
			{
				if (Jump(current->iX + 1, current->iY + 1, en_DIR_DR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			}
			return TRUE;
		}
		// DR
		if ((current->pParent)->iY - current->iY < 0)	// 부모가 UL 방향
		{
			if (Jump(current->iX + 1, current->iY + 1, en_DIR_DR, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX + 1, current->iY + 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 14)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX, current->iY + 1, en_DIR_DD, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX, current->iY + 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX + 1, current->iY, en_DIR_RR, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX + 1, current->iY))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (!isWalkable(current->iX - 1, current->iY) && isWalkable(current->iX - 1, current->iY + 1))
			{
				if (Jump(current->iX - 1, current->iY + 1, en_DIR_DL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			}
			if (!isWalkable(current->iX, current->iY - 1) && isWalkable(current->iX + 1, current->iY - 1))
			{
				if (Jump(current->iX + 1, current->iY - 1, en_DIR_UR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			}
			return TRUE;
		}

	}
	if ((current->pParent)->iX - current->iX > 0)
	{
		// LL
		if ((current->pParent)->iY - current->iY == 0)	// 부모가 RR 방향이면 
		{
			if (Jump(current->iX - 1, current->iY, en_DIR_LL, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX - 1, current->iY))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (!isWalkable(current->iX, current->iY - 1) && isWalkable(current->iX - 1, current->iY - 1))
				if (Jump(current->iX - 1, current->iY - 1, en_DIR_UL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}

					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			if (!isWalkable(current->iX, current->iY + 1) && isWalkable(current->iX - 1, current->iY + 1))
				if (Jump(current->iX - 1, current->iY + 1, en_DIR_DL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			return TRUE;
		}
		// UL
		if ((current->pParent)->iY - current->iY > 0)	// 부모가 DR 방향
		{
			if (Jump(current->iX - 1, current->iY - 1, en_DIR_UL, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX - 1, current->iY - 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 14)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX, current->iY - 1, en_DIR_UU, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX, current->iY - 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX - 1, current->iY, en_DIR_LL, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX - 1, current->iY))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}

			if (!isWalkable(current->iX + 1, current->iY) && isWalkable(current->iX + 1, current->iY - 1))
				if (Jump(current->iX + 1, current->iY - 1, en_DIR_UR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			if (!isWalkable(current->iX, current->iY + 1) && isWalkable(current->iX - 1, current->iY + 1))
				if (Jump(current->iX - 1, current->iY + 1, en_DIR_DL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			return TRUE;
		}
		// DL
		if ((current->pParent)->iY - current->iY < 0)	// 부모가 UR 방향
		{
			if (Jump(current->iX - 1, current->iY + 1, en_DIR_DL, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX - 1, current->iY + 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 14)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX - 1, current->iY, en_DIR_LL, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX - 1, current->iY))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}
			if (Jump(current->iX, current->iY + 1, en_DIR_DD, &iJumpX, &iJumpY) &&
				!SearchCloselst(current->iX, current->iY + 1))
			{
				pTemp = SearchOpenlst(iJumpX, iJumpY);
				if (!pTemp)
				{
					pTemp = new stArea(iJumpX, iJumpY, current);
					pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
					pTemp->H = GetDistance(pTemp, &_DestPos);
					pTemp->F = pTemp->G + pTemp->H;
					_Openlst.push_front(pTemp);
				}
				else
				{
					if (pTemp->G > current->G + 10)
					{
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 10;
						pTemp->F = pTemp->G + pTemp->H;
						pTemp->pParent = current;
					}
				}
			}

			if (!isWalkable(current->iX, current->iY - 1) && isWalkable(current->iX - 1, current->iY - 1))
				if (Jump(current->iX - 1, current->iY - 1, en_DIR_UL, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX - 1, current->iY - 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			if (!isWalkable(current->iX + 1, current->iY) && isWalkable(current->iX + 1, current->iY + 1))
				if (Jump(current->iX + 1, current->iY + 1, en_DIR_DR, &iJumpX, &iJumpY) &&
					!SearchCloselst(current->iX + 1, current->iY + 1))
				{
					pTemp = SearchOpenlst(iJumpX, iJumpY);
					if (!pTemp)
					{
						pTemp = new stArea(iJumpX, iJumpY, current);
						pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
						pTemp->H = GetDistance(pTemp, &_DestPos);
						pTemp->F = pTemp->G + pTemp->H;
						_Openlst.push_front(pTemp);
					}
					else
					{
						if (pTemp->G > current->G + 14)
						{
							pTemp->G = (float)(abs(current->iX - _StartPos.iX) * 10) + (abs(current->iY - _StartPos.iY) * 10);//current->G + 14;
							pTemp->F = pTemp->G + pTemp->H;
							pTemp->pParent = current;
						}
					}
				}
			return TRUE;
		}
	}

	return FALSE;
}

float mylib::CJPS::GetDistance(const stArea * from, const stArea * to)
{
	return 10 * (abs(from->iX - to->iX) + abs(from->iY - to->iY));
}

bool mylib::CJPS::compFormula(const stArea * a, const stArea * b)
{
	return a->F < b->F;
}
