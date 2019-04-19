#ifndef __BRESENHA__
#define __BRESENHA__

namespace mylib
{
	class CBresenham
	{
	public:
		enum en_RETURN_SETPOSITION
		{
			en_POS_SAME = 0,
			en_LEN_SAME,
			en_LEN_Y_LESS,
			en_LEN_X_LESS,
		};

		CBresenham(int iStartX, int iStartY, int iDestX, int iDestY);
		~CBresenham();
		int SetPosition(int iStartX, int iStartY, int iDestX, int iDestY);
		bool GetNext(int &iX, int &iY);
		bool PeekNext(int &iX, int &iY);

	private:
		POINT _stStart;
		POINT _stDest;
		int _iDistanceX;
		int _iDistanceY;
		std::list<POINT> _Pointlst;
	};
}

#endif