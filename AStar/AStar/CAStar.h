#ifndef __PATH_H__
#define __PATH_H__
#include <windowsx.h>

namespace mylib
{
	class CAStar
	{
	public:
		struct stArea
		{
			stArea(int x = 0, int y = 0, stArea* parent = nullptr);
			stArea(stArea &copy);
			int iX, iY;
			float G;	// Move Cnt
			float H;	// Distance to Destinationn Point
			float F;	// G + H
			stArea* pParent;
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
		CAStar(int iMapWidth = 90, int iMapHeight = 40);
		CAStar(CMap& pMap);
		virtual ~CAStar();

		// Set MousePos to MapPos 
		void SetStart(POINT mousepos);
		void SetDest(POINT mousepos);


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
		bool PathFind();
		/// * Process (need Outer Loop)
		void Clear_Process();
		bool PathFind_Process();


		////////////////////////////////////////////////////////////
		// 길 표시 (WinAPI 전용)
		//
		// Parameters : (HDC) 
		// Return :
		////////////////////////////////////////////////////////////
		void DrawPath(HDC hdc);

	protected:
		stArea* SearchOpenlst(int iX, int iY);
		stArea* SearchCloselst(int iX, int iY);

		BOOL CheckTile(int iX, int iY);
		BOOL CheckTile_Around(stArea * current);

		float mylib::CAStar::GetDistance(const stArea * from, const stArea * to);

		static bool compFormula(const stArea* a, const stArea *b);

	public:
		CMap* _pCMap;
		
		std::list<stArea*> _Openlst;
		std::list<stArea*> _Closelst;

		stArea _StartPos;
		stArea _DestPos;
		stArea * _pStartPos_Temp;

		// PathFind_Process() Status
		bool _bStart;	

		// PathFind Result
		bool _isRoute;
	};
}
#endif __PATH_H__