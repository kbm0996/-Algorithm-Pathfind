#ifndef __BRESENHAM__
#define __BRESENHAM__

namespace mylib
{
	class CBresenham
	{
	public:
		CBresenham(int iStartX, int iStartY, int iDestX, int iDestY);
		~CBresenham();
		int SetPosition(int iStartX, int iStartY, int iDestX, int iDestY);
		bool GetNext(int &iX, int &iY);
		bool PeekNext(int &iX, int &iY);

	private:
		struct Point
		{
			Point();
			Point(int X, int Y);
			int iX;
			int iY;
		};

		Point m_stStart;
		Point m_stDest;
		int m_iDistanceX;
		int m_iDistanceY;
		std::list<Point*> m_PointList;
	};
}

#endif