#include "renderer.h"


Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::Initialize(const HWND overlay) {
	this->m_isRunning = true;

	if (!overlay) {
		return false;
	}
	else this->overlayHWND = overlay;

	if (!this->r_InitDirectX()) {
		Utility::PrintError("Failed to initialize DirectX");
		return false;
	}

	this->Esp = new ESP(*this);
	this->ViewMatrix = new std::vector<float>(16);
	this->LocalEnt = new Entity();

	return true;
}

void Renderer::Run() {
	while (this->m_isRunning) {
		this->r_RenderFrame();
	}
}

bool Renderer::r_InitDirectX() {
	this->d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferWidth = this->WindowDimensions.x;
	d3dpp.BackBufferHeight = this->WindowDimensions.y;
	d3dpp.hDeviceWindow = this->overlayHWND;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	HRESULT DeviceCreated = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->overlayHWND, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &this->d3ddev);
	if (DeviceCreated != S_OK) {
		return false;
	}

	D3DXCreateLine(this->d3ddev, &this->m_Line);
	D3DXCreateFont(this->d3ddev, 30, 0, FW_BOLD, -1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &this->m_Font);
	D3DXCreateFont(this->d3ddev, 10, 0, FW_BOLD, -1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &this->m_PlayerNameFont);

	return true;
}

void Renderer::r_RenderFrame() {
	this->d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	this->d3ddev->BeginScene();

	std::string LocalTeam = this->LocalEnt->GetTeam();

	for (auto& curr : *this->Entities) {
		if (!curr->IsVisible || curr->IsDead)
			continue;

		if (curr->GetTeam() == LocalTeam) {
			this->Esp->DrawEspBox(curr->EntityHeadScreen, curr->EntityBodyScreen, D3DCOLOR_ARGB(255, 73, 243, 73));
		}
		else this->Esp->DrawEspBox(curr->EntityHeadScreen, curr->EntityBodyScreen, D3DCOLOR_ARGB(255, 243, 73, 73));

		this->Esp->DrawHealthBar(curr->EntityHeadScreen, curr->EntityBodyScreen, curr->Health);
		this->Esp->DrawArmorBar(curr->EntityHeadScreen, curr->EntityBodyScreen, curr->Armor);
		this->Esp->DrawPlayerName(curr->EntityBodyScreen, curr->GetName());
	}

	this->d3ddev->EndScene();
	this->d3ddev->Present(NULL, NULL, NULL, NULL);
}

bool Renderer::r_CleanupDirectX() {
	this->d3ddev->Release();
	this->d3d->Release();
	return true;
}

void Renderer::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x, y, x + w, y + h };
	this->d3ddev->Clear(1, &rect, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	D3DXVECTOR2 line[2];
	line[0] = D3DXVECTOR2(x1, y1);
	line[1] = D3DXVECTOR2(x2, y2);
	this->m_Line->SetWidth(thickness);
	this->m_Line->Draw(line, 2, color);
}

void Renderer::DrawLine(Vector2 p1, Vector2 p2, int thickness, D3DCOLOR color) {
	D3DXVECTOR2 line[2];
	line[0] = D3DXVECTOR2(p1.x, p1.y);
	line[1] = D3DXVECTOR2(p2.x, p2.y);
	this->m_Line->SetWidth(thickness);
	this->m_Line->Draw(line, 2, color);
}

void Renderer::ESP::DrawEspBox(const Vector2& HeadScreen, const Vector2& FeetScreen, D3DCOLOR Color) {
	float PlayerHeight = FeetScreen.y - HeadScreen.y;
	if (PlayerHeight < 0.0)
		return;
	
	static Vector2 TopLeft, TopRight, BottomLeft, BottomRight;
	TopLeft.x = HeadScreen.x - (PlayerHeight / 3); TopLeft.y = HeadScreen.y;
	TopRight.x = HeadScreen.x + (PlayerHeight / 3); TopRight.y = HeadScreen.y;
	BottomLeft.x = FeetScreen.x - (PlayerHeight / 3); BottomLeft.y = FeetScreen.y;
	BottomRight.x = FeetScreen.x + (PlayerHeight / 3); BottomRight.y = FeetScreen.y;

	this->renderer.DrawLine(TopLeft, TopRight, 2, Color);
	this->renderer.DrawLine(TopRight, BottomRight, 2, Color);
	this->renderer.DrawLine(BottomRight, BottomLeft, 2, Color);
	this->renderer.DrawLine(BottomLeft, TopLeft, 2, Color);
	this->renderer.DrawLine(HeadScreen, FeetScreen, PlayerHeight / 1.5, D3DCOLOR_ARGB(175, 10, 10, 10));
}

void Renderer::ESP::DrawHealthBar(const Vector2& HeadScreen, const Vector2& FeetScreen, const int& Health) {
	float PlayerHeight = FeetScreen.y - HeadScreen.y;
	float HealthBarHeight = PlayerHeight / 100 * Health;

	static Vector2 HealthBarBottom, HealthBarTop;
	HealthBarBottom.x = FeetScreen.x - (PlayerHeight / 2);
	HealthBarBottom.y = FeetScreen.y;
	HealthBarTop.x = HeadScreen.x - (PlayerHeight / 2);
	HealthBarTop.y = FeetScreen.y - HealthBarHeight;

	int Red = 255 - (Health * 2.5);
	int Green = 255 + (Health * 2.5);
	D3DCOLOR Color = D3DCOLOR_ARGB(255, Red, Green, 0);

	this->renderer.DrawLine(HealthBarBottom, HealthBarTop, 1, Color);
}

void Renderer::ESP::DrawArmorBar(const Vector2& HeadScreen, const Vector2& FeetScreen, const int& Armor) {
	float PlayerHeight = FeetScreen.y - HeadScreen.y;
	float ArmorBarHeight = PlayerHeight / 100 * Armor;

	static Vector2 ArmorBarBottom, ArmorBarTop;
	ArmorBarBottom.x = FeetScreen.x + (PlayerHeight / 2);
	ArmorBarBottom.y = FeetScreen.y;
	ArmorBarTop.x = HeadScreen.x + (PlayerHeight / 2);
	ArmorBarTop.y = FeetScreen.y - ArmorBarHeight;

	D3DCOLOR Color = D3DCOLOR_ARGB(255, 0, 0, 255);

	this->renderer.DrawLine(ArmorBarBottom, ArmorBarTop, 1, Color);
}

void Renderer::ESP::DrawPlayerName(const Vector2& FeetScreen, const std::string& Name) {
	RECT rect;
	SetRect(&rect, FeetScreen.x, FeetScreen.y, FeetScreen.x, FeetScreen.y);
	this->renderer.m_PlayerNameFont->DrawTextA(NULL, Name.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
}
