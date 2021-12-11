#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "globals.h"

namespace draw
{
	bool deviceInit(LPDIRECT3DDEVICE9 d3d);

	//Shapes and drawing utility
	void drawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void drawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);
	void drawX(int x, int y, int size, D3DCOLOR color);
	void drawSimpleLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
	void drawCircle(int x, int y, int radius, int thickness, D3DCOLOR color);

	//fonts
	void drawHeaderText(LPCSTR text, int x, int y, D3DCOLOR color);
	void drawMenuText(LPCSTR text, int x, int y, D3DCOLOR color);
	void drawNameText(const char* text, int x, int y, D3DCOLOR color);

	//Esp specific
	void drawEspBox(Vec2 feet, Vec2 head, int health, D3DCOLOR color);
	void drawInfoBox(Vec2 src, LPCSTR name, LPCSTR distance, D3DCOLOR color);

	//Interactive visuals
	void menuManager();
	bool Checkbox(int x, int y, int mouseX, int mouseY, bool status, D3DCOLOR color);
}