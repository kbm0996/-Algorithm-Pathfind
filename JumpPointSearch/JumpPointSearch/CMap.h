#ifndef __pMap__
#define __pMap__

namespace mylib
{
	class CMap
	{
	public:
		enum enTILE
		{
			en_TILE_WIDTH = 15,
			en_TILE_HEIGHT = 15
		};

		CMap(int iWidth = 0, int iHeight = 0);
		CMap(const CMap &cpy);
		virtual ~CMap();

		void DrawMap(HDC hdc);

		bool CheckObstacle(POINT mousepos);
		void SetObstacle(POINT mousepos, bool isObstacle);
		void ResetObstacle();

		POINT GetTilePos(POINT mousepos);

		int _iWidth;
		int _iHeight;
		char** _pMap;
	};
}

#endif // !__pMap__
