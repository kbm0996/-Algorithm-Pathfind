#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 10

#define KEY_LEFT	0x4b
#define KEY_RIGHT	0x4d
#define KEY_UP		0x48
#define KEY_DOWN	0x50
#define KEY_ENTER	0x0d

// Stack
struct stData {
	int iX;
	int iY;
};

bool Push(int iX, int iY);
bool Pop(int *pX, int *pY);

// Rendering
void Init(char pMap[][SCREEN_WIDTH]);
void Clear();
void Draw();
void PaintChar(int x, int y, char chData);

// Key Process
void KeyProcess();
#endif