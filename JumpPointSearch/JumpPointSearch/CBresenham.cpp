#include "stdafx.h"



mylib::CBresenham::CBresenham(int iStartX, int iStartY, int iDestX, int iDestY)
{
	SetPosition(iStartX, iStartY, iDestX, iDestY);
}


void mylib::CBresenham::SetPosition(int iStartX, int iStartY, int iDestX, int iDestY)
{
	_ptStart.x = iStartX;
	_ptStart.y = iStartY;
	_ptDest.x = iDestX;
	_ptDest.y = iDestY;
	_iMargin = 0;
	
	_iDistanceX = abs(_ptDest.x - _ptStart.x);
	_iDistanceY = abs(_ptDest.y - _ptStart.y);

	if (_iDistanceX == 0)
		_iIncreaseX = 0;
	else
		_iIncreaseX = (iDestX - iStartX) / _iDistanceX;
	
	if (_iDistanceY == 0)
		_iIncreaseY = 0;
	else
		_iIncreaseY = (iDestY - iStartY) / _iDistanceY;
}

bool mylib::CBresenham::GetNext(int * x, int * y)
{
	if (_ptStart.x == _ptDest.x && _ptStart.y == _ptDest.y)
		return true;

	if (_iDistanceX >= _iDistanceY)
	{
		_ptStart.x += _iIncreaseX;
		_iMargin += _iDistanceY;
		if (_iMargin >= _iDistanceX / 2)
		{
			_ptStart.y += _iIncreaseY;
			_iMargin -= _iDistanceX;
		}
	}
	else
	{
		_ptStart.y += _iIncreaseY;
		_iMargin += _iDistanceX;
		if (_iMargin >= _iDistanceY / 2)
		{
			_ptStart.x += _iIncreaseX;
			_iMargin -= _iDistanceY;
		}
	}

	*x = _ptStart.x;
	*y = _ptStart.y;

	return false;
}

bool mylib::CBresenham::PeekNext(int * x, int * y)
{
	if (_ptStart.x == _ptDest.x && _ptStart.y == _ptDest.y)
		return true;

	POINT	ptStart_copy = _ptStart;
	int		iMargin_copy = _iMargin;
	if (_iDistanceX >= _iDistanceY)
	{
		ptStart_copy.x += _iIncreaseX;
		iMargin_copy += _iDistanceY;
		if (iMargin_copy >= _iDistanceX / 2)
		{
			ptStart_copy.y += _iIncreaseY;
			iMargin_copy -= _iDistanceX;
		}
	}
	else
	{
		ptStart_copy.y += _iIncreaseY;
		iMargin_copy += _iDistanceX;
		if (iMargin_copy >= _iDistanceY / 2)
		{
			ptStart_copy.x += _iIncreaseX;
			iMargin_copy -= _iDistanceY;
		}
	}

	*x = ptStart_copy.x;
	*y = ptStart_copy.y;

	return false;
}
