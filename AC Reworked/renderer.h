#ifndef AC_RENDERER_H
#define AC_RENDERER_h

#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "vector.h"
#include "utility.h"
#include "entity.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


class Renderer {
public:
	Renderer();
	~Renderer();

	bool Initialize(const HWND overlay = 0);
	void Run();

	inline void r_SetWindowDimensions(const Vector2& Dimensions) { this->WindowDimensions = Dimensions; }

	inline void r_SetViewMatrix(std::vector<float>* ViewMatrix) { this->ViewMatrix = ViewMatrix; }
	inline void r_SetEntities(std::vector<Entity*>* Entities, Entity* LocalEnt) { this->Entities = Entities; this->LocalEnt = LocalEnt; }
	
private:
	//DirectX Stuff
	bool r_InitDirectX();
	void r_RenderFrame();
	bool r_CleanupDirectX();

	//Drawing Shapes
	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
	void DrawLine(Vector2 p1, Vector2 p2, int thickness, D3DCOLOR color);


	Vector2 WindowDimensions;
	HINSTANCE hInstance;
	HWND overlayHWND;

	struct ESP {
		ESP(Renderer& parent): renderer(parent) {}

		void DrawEspBox(const Vector2& HeadScreen, const Vector2& FeetScreen, D3DCOLOR Color);
		void DrawHealthBar(const Vector2& HeadScreen, const Vector2& FeetScreen, const int& Health);
		void DrawArmorBar(const Vector2& HeadScreen, const Vector2& FeetScreen, const int& Armor);
		void DrawPlayerName(const Vector2& FeetScreen, const std::string& Name);

		Renderer& renderer;
	};

protected:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;
	ID3DXLine* m_Line;
	ID3DXFont* m_Font;
	ID3DXFont* m_PlayerNameFont;

private:
	ESP* Esp;
	bool m_isRunning;

	//To Draw
	std::vector<float>* ViewMatrix;
	Entity* LocalEnt;
	std::vector<Entity*>* Entities;
};


#endif //AC_RENDERER_H

