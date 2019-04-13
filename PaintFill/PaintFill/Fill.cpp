#include "Fill.h"

char g_Buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

int g_iCurX;
int g_iCurY;

stData Stack[SCREEN_HEIGHT*SCREEN_WIDTH];
int g_iTop = 0;

bool Push(int iX, int iY)
{
	if (g_iTop >= SCREEN_HEIGHT*SCREEN_WIDTH)
		return false;
	Stack[g_iTop].iX = iX;
	Stack[g_iTop].iY = iY;
	++g_iTop;
	return true;
}

bool Pop(int *pX, int *pY)
{
	if (g_iTop == 0)
		return false;
	--g_iTop;
	*pX = Stack[g_iTop].iX;
	*pY = Stack[g_iTop].iY;
	return true;
}

void Draw()
{
	for (int iCntH = 0; iCntH < SCREEN_HEIGHT; ++iCntH)
	{
		for (int iCntW = 0; iCntW < SCREEN_WIDTH; ++iCntW)
		{
			if (iCntH == g_iCurY)
			{
				if (iCntW == g_iCurX)
				{
					printf("*");
				}
				else
					printf("%c", g_Buffer[iCntH][iCntW]);
			}
			else
				printf("%c", g_Buffer[iCntH][iCntW]);
		}
		printf("\n");
	}
}

void Init(char pMap[][SCREEN_WIDTH])
{
	memcpy_s(g_Buffer, sizeof(g_Buffer), pMap, SCREEN_HEIGHT * SCREEN_WIDTH);
}

void Clear()
{
	ZeroMemory(g_Buffer, sizeof(g_Buffer));
}


void PaintChar(int x, int y, char chData)
{
	//-----------------------------------
	// Ver. Recursive Function
	//-----------------------------------
	/*if (g_Buffer[y][x] == '0')
	{
	g_Buffer[y][x] = chData;
	DrawBuffer();
	PaintChar(x - 1, y);
	PaintChar(x + 1, y);
	PaintChar(x, y - 1);
	PaintChar(x, y + 1);
	}
	else
	return;*/

	//-----------------------------------
	// Ver. Stack 
	//-----------------------------------
	while (1)
	{
		if (g_Buffer[y][x] == '0')
		{
			if (Push(x, y + 1))
				g_Buffer[y][x] = chData;
			if (Push(x, y - 1))
				g_Buffer[y][x] = chData;
			if (Push(x + 1, y))
				g_Buffer[y][x] = chData;
			if (Push(x - 1, y))
				g_Buffer[y][x] = chData;
		}
		else
		{
			if (Pop(&x, &y))
				continue;
			else
				break;
		}

		// 채워지는 과정
		system("cls");
		Draw();
	}
}

void KeyProcess()
{
	char chKey;

	while (1)
	{
		chKey = _getch();
		switch (chKey)
		{
		case KEY_RIGHT:
			if (g_iCurX < SCREEN_WIDTH - 1)
				++g_iCurX;
			return;

		case KEY_LEFT:
			if (g_iCurX > 0)
				--g_iCurX;
			return;

		case KEY_UP:
			if (g_iCurY > 0)
				--g_iCurY;
			return;

		case KEY_DOWN:
			if (g_iCurY < SCREEN_HEIGHT - 1)
				++g_iCurY;
			return;

		case KEY_ENTER:
			PaintChar(g_iCurX, g_iCurY, '*');
			Sleep(2000);
			return;
		}
	}
}