#ifndef __PATH_H__
#define __PATH_H__
#include <windowsx.h>

namespace mylib
{
	class CAStar
	{
	public:
		struct stTile
		{
			stTile(int x = 0, int y = 0, stTile* parent = nullptr);
			stTile(stTile &copy);
			int iX, iY;
			float G;	// Move Cnt
			float H;	// Distance to Destinationn Point
			float F;	// G + H
			stTile* pParent;
		};

		CAStar(int iMapWidth = 90, int iMapHeight = 40);
		CAStar(CMap& pMap);
		virtual ~CAStar();

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
		stTile* SearchOpenlst(int iX, int iY);
		stTile* SearchCloselst(int iX, int iY);

		BOOL CheckTile(int iX, int iY);
		BOOL CheckTile_Around(stTile * current);

		float mylib::CAStar::GetDistance(const stTile * from, const stTile * to);

	private:
		static bool compFormula(const stTile* a, const stTile *b);

		CMap* _pMap;
		std::list<stTile*> _Openlst;
		std::list<stTile*> _Closelst;

		stTile _StartPos;
		stTile _DestPos;
		stTile * _pStartPos_Temp;
		
		bool _bStart;	
		bool _isRoute;
	};
}
#endif __PATH_H__