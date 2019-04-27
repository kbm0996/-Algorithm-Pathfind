#ifndef __CMAP__
#define __CMAP__

namespace mylib
{
	class CMap
	{
	public:
		enum enTILE
		{
			// MAP SIZE
			en_TILE_SIZE_WIDTH = 15,
			en_TILE_SIZE_HEIGHT = 15,

			// TILE TYPE
			en_TILE_TYPE_OBSTACLE = 0,
			en_TILE_TYPE_NORMAL,

			// TILE COLOR
			/// OBSTACLE
			en_TILE_COLOR_OBS_R = 0x49,
			en_TILE_COLOR_OBS_G = 0x49,
			en_TILE_COLOR_OBS_B = 0x49,
			/// NORMAL
			en_TILE_COLOR_NORMAL_R = 0xff,
			en_TILE_COLOR_NORMAL_G = 0xff,
			en_TILE_COLOR_NORMAL_B = 0xff
		};

		struct stTILE
		{
			BYTE byType;
			BYTE byR, byG, byB;
			bool bMark;
		};

		////////////////////////////////////////////////////////////
		// ������, �Ҹ���
		//  ���� ũ�� ������ ������ �Բ� ����
		//
		// Parameters : (int) �ʺ�
		//				(int) ����
		// Return :		
		////////////////////////////////////////////////////////////
		CMap(int iWidth = 0, int iHeight = 0);
		CMap(const CMap &cpy);
		virtual ~CMap();


		////////////////////////////////////////////////////////////
		// ���� ǥ�� (WinAPI ����)
		//
		// Parameters : (HDC) 
		// Return :
		////////////////////////////////////////////////////////////
		void DrawMap(HDC hdc);

		
		////////////////////////////////////////////////////////////
		// Ÿ�� ���� �˻�
		//
		// Parameters : (POINT) ���콺 ��ǥ 
		// Return :		(int) Ÿ�� ����
		////////////////////////////////////////////////////////////
		int CheckObstacle(POINT mousepos);


		////////////////////////////////////////////////////////////
		// ��ֹ� ����
		//
		// Parameters : (POINT) ���콺 ��ǥ,  (bool) ��ֹ� ����
		// Return :		
		////////////////////////////////////////////////////////////
		void SetObstacle(POINT mousepos, bool isObstacle);


		////////////////////////////////////////////////////////////
		// ��ֹ� ����
		//
		// Parameters : 
		// Return :		
		////////////////////////////////////////////////////////////
		void ResetObstacle();


		////////////////////////////////////////////////////////////
		// Jump ���� ���� �ʱ�ȭ
		//
		// Parameters : 
		// Return :		
		////////////////////////////////////////////////////////////
		void ResetMark();


		////////////////////////////////////////////////////////////
		// Ÿ�� ��ǥ ����(���콺 ��ǥ �� ������ ��ǥ)
		//
		// Parameters : (POINT) ���콺 ��ǥ
		// Return :		
		////////////////////////////////////////////////////////////
		POINT GetTilePos(POINT mousepos);

		int _iWidth;
		int _iHeight;
		stTILE** _pMap;
	};
}

#endif // !__CMAP__
