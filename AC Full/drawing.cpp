#include "drawing.h"

LPDIRECT3DDEVICE9 dev;
ID3DXFont* headerFont;
ID3DXFont* menuFont;
ID3DXFont* nameFont;

int menuX = 400, menuY = 150;

bool draw::deviceInit(LPDIRECT3DDEVICE9 d3d)
{
	if (!d3d)
		return false;

	dev = d3d;
    D3DXCreateFont(dev, 30, 0, FW_BOLD, -1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &headerFont);

	return true;
}


//-----------------------------------------------------------------------------------------------------


void draw::drawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	dev->Clear(1, &rect, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void draw::drawX(int x, int y, int size, D3DCOLOR color)
{
	draw::drawSimpleLine(x - (size / 2), y, x + (size / 2), y, 3, color);
	draw::drawSimpleLine(x, y - (size / 2), x, y + (size / 2), 3, color);
}

void draw::drawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color)
{
	ID3DXLine* LineL;
	D3DXCreateLine(dev, &LineL);
	D3DXVECTOR2 line[2];
	line[0] = D3DXVECTOR2(src.x, src.y);
	line[1] = D3DXVECTOR2(dst.x, dst.y);
	LineL->SetWidth(thickness);
	LineL->Draw(line, 2, color);
	LineL->Release();
}

void draw::drawSimpleLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	ID3DXLine* LineL;
	D3DXCreateLine(dev, &LineL);
	D3DXVECTOR2 line[2];
	line[0] = D3DXVECTOR2(x1, y1);
	line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(line, 2, color);
	LineL->Release();
}

void draw::drawCircle(int x, int y, int radius, int thickness, D3DCOLOR color)
{
	int prevX = x - radius, prevY = y;

	for (int i = 0; i < 128; i++)
	{
		draw::drawSimpleLine(prevX, prevY, prevX + 1, prevY + i, thickness, color);
		prevX += i / 3;
	}
}


//-------------------------------------------------------------------------------------------------


void draw::drawHeaderText(LPCSTR text, int x, int y, D3DCOLOR color)
{
	if (!headerFont)
		D3DXCreateFont(dev, 30, 0, FW_BOLD, -1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &headerFont);

	RECT rect;
	SetRect(&rect, x, y, x, y);
	headerFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void draw::drawMenuText(LPCSTR text, int x, int y, D3DCOLOR color)
{
	if (!menuFont)
		D3DXCreateFont(dev, 18, 0, FW_BOLD, -1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &menuFont);

	RECT rect;
	SetRect(&rect, x, y, x, y);
	menuFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void draw::drawNameText(const char* text, int x, int y, D3DCOLOR color)
{
	if (!nameFont)
		D3DXCreateFont(dev, 8, 0, FW_NORMAL, -1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &nameFont);

	RECT rect;
	SetRect(&rect, x, y, x, y);
	nameFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}


//-----------------------------------------------------------------------------------------


void draw::drawEspBox(Vec2 feet, Vec2 head, int health, D3DCOLOR color)
{
	static float height;
	height = feet.y - head.y;
	static Vec2 tl, tr, br, bl, ht, hb;
	tl.x = head.x - (height / 3);
	tl.y = head.y;
	tr.x = head.x + (height / 3);
	tr.y = head.y;
	br.x = feet.x + (height / 3);
	br.y = feet.y;
	bl.x = feet.x - (height / 3);
	bl.y = feet.y;
	hb.x = feet.x - (height / 3) - 3;
	hb.y = feet.y;
	ht.x = head.x - (height / 3) - 3;
	ht.y = feet.y - height / 100 * health;

	draw::drawLine(tl, tr, 2, color);
	draw::drawLine(tr, br, 2, color);
	draw::drawLine(br, bl, 2, color);
	draw::drawLine(bl, tl, 2, color);
	draw::drawLine(hb, ht, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
	draw::drawLine(feet, head, height * .67, D3DCOLOR_ARGB(100, 100, 100, 100));

	//HeadBox
	draw::drawSimpleLine(head.x, head.y - (height * .1), head.x, head.y + (height * .1), height * .2, D3DCOLOR_ARGB(150, 240, 100, 100));
}

void draw::drawInfoBox(Vec2 src, LPCSTR name, LPCSTR distance, D3DCOLOR color)
{
	draw::drawSimpleLine(src.x, src.y - 10, src.x, src.y - 20, 40, D3DCOLOR_ARGB(200, 30, 30, 30));
	draw::drawSimpleLine(src.x, src.y - 10, src.x, src.y - 7, 40, D3DCOLOR_ARGB(255, 90, 90, 200));
	draw::drawNameText(name, src.x - 10, src.y - 19, D3DCOLOR_ARGB(255, 255, 255, 255));
	draw::drawNameText(distance, src.x + 20, src.y - 19, D3DCOLOR_ARGB(255, 255, 255, 255));
}


//---------------------------------------------------------------------------------------------------


void draw::menuManager()
{
	static POINT Mouse;
	GetCursorPos(&Mouse);


	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (Mouse.x >= menuX + windowX - 250 && Mouse.x <= menuX + windowX + 250 && Mouse.y >= menuY + windowY && Mouse.y <= menuY + windowY + 50)
		{
			menuX = Mouse.x - windowX;
			menuY = Mouse.y - windowY - 29;
		}
	}


	Mouse.x -= windowX;
	Mouse.y -= windowY;

	draw::drawSimpleLine(menuX, menuY, menuX, menuY + 350, 500, D3DCOLOR_ARGB(240, 30, 30, 30));

	draw::drawHeaderText("IMGui's for Skids!", menuX, menuY + 11, D3DCOLOR_ARGB(255, 255, 255, 255));
	draw::drawSimpleLine(menuX - 240, menuY + 50, menuX + 240, menuY + 50, 3, D3DCOLOR_ARGB(170, 230, 230, 230));

	//Left column
	draw::drawMenuText("Snaplines", menuX - 200, menuY + 70, D3DCOLOR_ARGB(255, 240, 240, 240));
	draw::Checkbox(menuX -140, menuY + 70, Mouse.x, Mouse.y, false, D3DCOLOR_ARGB(255, 240, 240, 240));

	draw::drawX(Mouse.x, Mouse.y, 22, D3DCOLOR_ARGB(255, 255, 255, 255));
}

bool draw::Checkbox(int x, int y, int mouseX, int mouseY, bool status, D3DCOLOR color)
{
	if (mouseX >= x && mouseX <= x + 20 && mouseY >= y && mouseY <= y + 20)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 1)
			return true;
		color = D3DCOLOR_ARGB(255, 180, 180, 180);
	}

	draw::drawFilledRect(x, y, 20, 20, color);
	return false;
}