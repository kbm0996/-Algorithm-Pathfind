#include "stdafx.h"



mylib::CBresenham::CBresenham(int iStartX, int iStartY, int iDestX, int iDestY)
{
	int iError = 0;
	int iX = 0;
	int iY = 0;
	POINT ptTemp;
	if (SetPosition(iStartX, iStartY, iDestX, iDestY) == en_LEN_Y_LESS)
	{
		for (iX = 1; iX <= _iDistanceX; ++iX)
		{
			iError += _iDistanceY * 2;
			if (_iDistanceX < iError)
			{
				iError -= _iDistanceX * 2;
				++iY;
			}
			if (iStartX < iDestX && iStartY < iDestY)
				ptTemp = { iStartX + iX, iStartY + iY };
			else if (iStartX > iDestX && iStartY > iDestY)
				ptTemp = { iStartX - iX, iStartY - iY };
			else if (iStartX < iDestX && iStartY > iDestY)
				ptTemp = { iStartX + iX, iStartY - iY };
			else
				ptTemp = { iStartX - iX, iStartY + iY };

			_Pointlst.push_back(ptTemp);
		}
	}
	else if (SetPosition(iStartX, iStartY, iDestX, iDestY) == en_LEN_X_LESS)
	{
		for (iY = 1; iY <= _iDistanceY; ++iY)
		{
			iError += _iDistanceX * 2;
			if (_iDistanceY < iError)
			{
				iError -= _iDistanceY * 2;
				++iX;
			}
			if (iStartX < iDestX && iStartY < iDestY)
				ptTemp = { iStartX + iX, iStartY + iY };
			else if (iStartX > iDestX && iStartY > iDestY)
				ptTemp = { iStartX - iX, iStartY - iY };
			else if (iStartX < iDestX && iStartY > iDestY)
				ptTemp = { iStartX + iX, iStartY - iY };
			else
				ptTemp = { iStartX - iX, iStartY + iY };

			_Pointlst.push_back(ptTemp);
		}
	}
	else if (SetPosition(iStartX, iStartY, iDestX, iDestY) == en_LEN_SAME)
	{
		for (iX = 0; iX <= _iDistanceX; ++iX)
		{
			if (iStartX < iDestX && iStartY < iDestY)
				ptTemp = { iStartX + iX, iStartY + iY };
			else if (iStartX > iDestX && iStartY > iDestY)
				ptTemp = { iStartX - iX, iStartY - iY };
			else if (iStartX < iDestX && iStartY > iDestY)
				ptTemp = { iStartX + iX, iStartY - iY };
			else
				ptTemp = { iStartX - iX, iStartY + iY };
			_Pointlst.push_back(ptTemp);
			++iY;
		}
	}
	else
	{
		if (iStartY == iDestY)
		{
			for (iX = 1; iX <= _iDistanceX; ++iX)
			{
				if (iStartX < iDestX)
					ptTemp = { iStartX + iX, iStartY };
				else
					ptTemp = { iStartX - iX, iStartY };
				_Pointlst.push_back(ptTemp);
			}
		}
		else
		{
			for (iY = 1; iY < _iDistanceY; ++iY)
			{
				if (iStartY < iDestY)
					ptTemp = { iStartX, iStartY + iY };
				else
					ptTemp = { iStartX, iStartY - iY };
				_Pointlst.push_back(ptTemp);
			}
		}
	}

}

mylib::CBresenham::~CBresenham()
{
	_Pointlst.clear();
}

int mylib::CBresenham::SetPosition(int iStartX, int iStartY, int iDestX, int iDestY)
{
	_stStart.x = iStartX;
	_stStart.y = iStartY;
	_stDest.x = iDestX;
	_stDest.y = iDestY;
	_iDistanceX = abs(_stStart.x - _stDest.x);
	_iDistanceY = abs(_stStart.y - _stDest.y);
	if (_stStart.x == _stDest.x || _stStart.y == _stDest.y)
		return en_POS_SAME;
	else if (_iDistanceX == _iDistanceY)
		return en_LEN_SAME;
	else if (_iDistanceX > _iDistanceY)
		return en_LEN_Y_LESS;
	else if (_iDistanceX < _iDistanceY)
		return en_LEN_X_LESS;
}
bool mylib::CBresenham::GetNext(int & iX, int & iY)
{
	if (_Pointlst.size() == 0)
		return false;
	POINT ptTemp = _Pointlst.front();
	_Pointlst.pop_front();
	iX = ptTemp.x;
	iY = ptTemp.y;
	return true;
}

bool mylib::CBresenham::PeekNext(int & iX, int & iY)
{
	if (_Pointlst.size() == 0)
		return false;
	POINT ptTemp = _Pointlst.front();
	iX = ptTemp.x;
	iY = ptTemp.y;
	return true;
}
