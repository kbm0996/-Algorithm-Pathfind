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
		// 생성자, 소멸자
		//  지도 크기 설정시 지도도 함께 생성
		//
		// Parameters : (int) 너비
		//				(int) 높이
		// Return :		
		////////////////////////////////////////////////////////////
		CMap(int iWidth = 0, int iHeight = 0);
		CMap(const CMap &cpy);
		virtual ~CMap();


		////////////////////////////////////////////////////////////
		// 지도 표시 (WinAPI 전용)
		//
		// Parameters : (HDC) 
		// Return :
		////////////////////////////////////////////////////////////
		void DrawMap(HDC hdc);

		
		////////////////////////////////////////////////////////////
		// 타일 종류 검사
		//
		// Parameters : (POINT) 마우스 좌표 
		// Return :		(int) 타일 종류
		////////////////////////////////////////////////////////////
		int CheckObstacle(POINT mousepos);


		////////////////////////////////////////////////////////////
		// 장애물 설정
		//
		// Parameters : (POINT) 마우스 좌표,  (bool) 장애물 여부
		// Return :		
		////////////////////////////////////////////////////////////
		void SetObstacle(POINT mousepos, bool isObstacle);


		////////////////////////////////////////////////////////////
		// 장애물 제거
		//
		// Parameters : 
		// Return :		
		////////////////////////////////////////////////////////////
		void ResetObstacle();


		////////////////////////////////////////////////////////////
		// Jump 영역 색상 초기화
		//
		// Parameters : 
		// Return :		
		////////////////////////////////////////////////////////////
		void ResetMark();


		////////////////////////////////////////////////////////////
		// 타일 좌표 변경(마우스 좌표 → 지도상 좌표)
		//
		// Parameters : (POINT) 마우스 좌표
		// Return :		
		////////////////////////////////////////////////////////////
		POINT GetTilePos(POINT mousepos);

		int _iWidth;
		int _iHeight;
		stTILE** _pMap;
	};
}

#endif // !__CMAP__
