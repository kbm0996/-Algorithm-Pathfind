#ifndef __BRESENHAM__
#define __BRESENHAM__

namespace mylib
{
	class CBresenham
	{
	public:
		////////////////////////////////////////////////////////////
		// ������, �Ҹ���
		//  ���ο��� SetPosition() ȣ��
		//
		// Parameters : (int) ���� x
		//				(int) ���� y
		//				(int) ��ǥ x
		//				(int) ��ǥ y
		// Return :		
		////////////////////////////////////////////////////////////
		CBresenham(int iStartX = 0, int iStartY = 0, int iDestX = 0, int iDestY = 0);
		~CBresenham() {}

		////////////////////////////////////////////////////////////
		// �극������ ������ ���� ��ǥ, ��ǥ ��ǥ ����
		//
		// Parameters : (int) ���� x
		//				(int) ���� y
		//				(int) ��ǥ x
		//				(int) ��ǥ y
		// Return :		
		////////////////////////////////////////////////////////////
		void SetPosition(int iStartX, int iStartY, int iDestX, int iDestY);
		
		////////////////////////////////////////////////////////////
		// ���� ��ǥ �̱�
		//
		// Parameters : (int) __out__ x
		//				(int) __out__ y
		// Return :		
		////////////////////////////////////////////////////////////
		bool GetNext(int * x, int * y);

		////////////////////////////////////////////////////////////
		// ���� ��ǥ ���
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