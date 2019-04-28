#ifndef __PATH_H__
#define __PATH_H__
#include <windowsx.h>

#define isWalkable(X,Y) (((X >= 0) && (X < _pCMap->_iWidth) && (Y >= 0) && (Y < _pCMap->_iHeight) && (_pCMap->_pMap[Y][X].byType == CMap::en_TILE_TYPE_NORMAL))? TRUE:FALSE)

namespace mylib
{
	class CJPS
	{
	public:
		enum en_DIR_JUMP
		{
			en_DIR_DL = 1,
			en_DIR_DD,
			en_DIR_DR,
			en_DIR_LL,
			en_DIR_RR = 6,
			en_DIR_UL,
			en_DIR_UU,
			en_DIR_UR
		};

		struct stArea
		{
			stArea(int x = 0, int y = 0, stArea* parent = nullptr, stArea * pParent_Bresenham = nullptr);
			stArea(stArea &copy);
			int iX, iY;
			float G;	// Move Cnt
			float H;	// Distance to Destinationn Point
			float F;	// G + H
			stArea* pParent;
			stArea* pParent_Bresenham;
		};

		////////////////////////////////////////////////////////////
		// 생성자, 소멸자
		//  1. 지도 크기 설정시 지도 생성
		//  2. 지도 클래스 연동
		//
		// Parameters : (int) 지도 너비
		//				(int) 지도 높이
		//				(CMap&) 연동할 지도 클래스
		// Return :		
		////////////////////////////////////////////////////////////
		CJPS(int iMapWidth = 90, int iMapHeight = 40);
		CJPS(CMap& pMap);
		virtual ~CJPS();

		// Se MousePos to MapPos 
		void SetStart(POINT mousepos);
		void SetDest(POINT mousepos);

		// Clear Openlist, Closelist
		void Clear();


		////////////////////////////////////////////////////////////
		// 길찾기 실행 (과정 생략, 과정 표시:외부 루프 필요)
		//
		// Parameters : (HDC) 
		//				(bool) 브레즌햄 적용 여부
		//				(int) 탐색 횟수 제한
		//				(bool) Jump 영역 색상 표시 여부
		// Return :		길찾기 성공 여부
		////////////////////////////////////////////////////////////
		/// * Non-Process
		bool PathFind(HDC hdc = NULL, bool bBresenham = true, int iCheckRangeCnt = 0, bool bDomainMark = false);
		/// * Process (need Outer Loop)
		bool PathFind_Process(HDC hdc, bool bBresenham = true, int iCheckRangeCnt = 0, bool bDomainMark = true);


		////////////////////////////////////////////////////////////
		// 길 표시 (WinAPI 전용)
		//
		// Parameters : (HDC) 
		// Return :
		////////////////////////////////////////////////////////////
		void DrawPath(HDC hdc);
		
	protected:
		stArea* SearchOpenlst(int x, int y);
		stArea* SearchCloselst(int x, int y);

		BOOL Jump(int x, int y, en_DIR_JUMP enDir, int * pOutX, int * pOutY, bool bDomainMark);
		BOOL CheckTile_Around(stArea * current, bool bDomainMark);

		void	RandColor_JumpDomain();
		void	GetBresenhamPath(stArea * pDestNode);
		float	GetDistance(const stArea * from, const stArea * to);
		static bool compFormula(const stArea * a, const stArea * b);


	public:
		CMap* _pCMap;

	protected:
		std::list<stArea*> _Openlst;
		std::list<stArea*> _Closelst;

		stArea _StartPos;
		stArea _DestPos;
		stArea * _pStartPos_Temp;
		stArea _DestPos_Mark;

		// PathFind_Process() Status
		bool _bStart;

		// PathFind Result
		bool _isRoute;

		// Jump Domain Color
		BYTE _byR;
		BYTE _byG;
		BYTE _byB;
	};
}
#endif __PATH_H__