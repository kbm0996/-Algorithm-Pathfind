#ifndef __BRESENHAM__
#define __BRESENHAM__

namespace mylib
{
	class CBresenham
	{
	public:
		////////////////////////////////////////////////////////////
		// 생성자, 소멸자
		//  내부에서 SetPosition() 호출
		//
		// Parameters : (int) 시작 x
		//				(int) 시작 y
		//				(int) 목표 x
		//				(int) 목표 y
		// Return :		
		////////////////////////////////////////////////////////////
		CBresenham(int iStartX = 0, int iStartY = 0, int iDestX = 0, int iDestY = 0);
		~CBresenham() {}

		////////////////////////////////////////////////////////////
		// 브레즌헴을 적용할 시작 좌표, 목표 좌표 설정
		//
		// Parameters : (int) 시작 x
		//				(int) 시작 y
		//				(int) 목표 x
		//				(int) 목표 y
		// Return :		
		////////////////////////////////////////////////////////////
		void SetPosition(int iStartX, int iStartY, int iDestX, int iDestY);
		
		////////////////////////////////////////////////////////////
		// 다음 좌표 뽑기
		//
		// Parameters : (int) __out__ x
		//				(int) __out__ y
		// Return :		
		////////////////////////////////////////////////////////////
		bool GetNext(int * x, int * y);

		////////////////////////////////////////////////////////////
		// 다음 좌표 얻기
		//
		// Parameters : (int) __out__ x
		//				(int) __out__ y
		// Return :		
		////////////////////////////////////////////////////////////
		bool PeekNext(int * x, int * y);

	private:
		POINT _ptStart;
		POINT _ptDest;
		int _iDistanceX;
		int _iDistanceY;
		int _iIncreaseX;
		int _iIncreaseY;
		int _iMargin;
	};
}
#endif