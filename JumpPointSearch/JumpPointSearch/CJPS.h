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
		// ������, �Ҹ���
		//  1. ���� ũ�� ������ ���� ����
		//  2. ���� Ŭ���� ����
		//
		// Parameters : (int) ���� �ʺ�
		//				(int) ���� ����
		//				(CMap&) ������ ���� Ŭ����
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
		// ��ã�� ���� (���� ����, ���� ǥ��:�ܺ� ���� �ʿ�)
		//
		// Parameters : (HDC) 
		//				(bool) �극���� ���� ����
		//				(int) Ž�� Ƚ�� ����
		//				(bool) Jump ���� ���� ǥ�� ����
		// Return :		��ã�� ���� ����
		////////////////////////////////////////////////////////////
		/// * Non-Process
		bool PathFind(HDC hdc = NULL, bool bBresenham = true, int iCheckRangeCnt = 0, bool bDomainMark = false);
		/// * Process (need Outer Loop)
		bool PathFind_Process(HDC hdc, bool bBresenham = true, int iCheckRangeCnt = 0, bool bDomainMark = true);


		////////////////////////////////////////////////////////////
		// �� ǥ�� (WinAPI ����)
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