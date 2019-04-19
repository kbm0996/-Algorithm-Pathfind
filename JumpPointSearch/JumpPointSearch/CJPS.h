#ifndef __PATH_H__
#define __PATH_H__
#include <windowsx.h>

namespace mylib
{
	class CJPS
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

		CJPS(int iMapWidth = 90, int iMapHeight = 40);
		CJPS(CMap& pMap);
		virtual ~CJPS();

		// Conversion MousePos to MapPos 
		void SetStart(POINT mousepos);
		void SetDest(POINT mousepos);

		// Non-Process
		bool PathFind();

		// Process (need Outer Loop)
		void Clear_Process();
		bool PathFind_Process();

		// Get MapPt
		CMap * GetMap();

		//////////////////////////////////////////
		// Rendering (need WinAPI)
		//////////////////////////////////////////
		void DrawPath(HDC hdc);

	protected:
		stArea* SearchOpenlst(int iX, int iY);
		stArea* SearchCloselst(int iX, int iY);

		BOOL CheckTile(int iX, int iY);
		BOOL CheckTile_Around(stArea * current);

		float mylib::CJPS::GetDistance(const stArea * from, const stArea * to);

	private:
		static bool compFormula(const stArea* a, const stArea *b);

		CMap* _pMap;
		std::list<stArea*> _Openlst;
		std::list<stArea*> _Closelst;

		stArea _StartPos;
		stArea _DestPos;
		stArea * _pStartPos_Temp;

		bool _bStart;
		bool _isRoute;
	};
}
#endif __PATH_H__