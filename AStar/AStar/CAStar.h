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
		// ������, �Ҹ���
		//  1. ���� ũ�� ������ ���� ����
		//  2. ���� Ŭ���� ����
		//
		// Parameters : (int) ���� �ʺ�
		//				(int) ���� ����
		//				(CMap&) ������ ���� Ŭ����
		// Return :		
		////////////////////////////////////////////////////////////
		CAStar(int iMapWidth = 90, int iMapHeight = 40);
		CAStar(CMap& pMap);
		virtual ~CAStar();

		// Set MousePos to MapPos 
		void SetStart(POINT mousepos);
		void SetDest(POINT mousepos);


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
		bool PathFind();
		/// * Process (need Outer Loop)
		void Clear_Process();
		bool PathFind_Process();


		////////////////////////////////////////////////////////////
		// �� ǥ�� (WinAPI ����)
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