#include "app.h"


bool App::Initialize(const HINSTANCE hInstance) {
	this->m_Window = new WindowManager();
	if (!this->m_Window->w_Initialize(hInstance)) {
		Utility::PrintError("Failed to initialize Window");
		return false;
	}

	std::thread(&WindowManager::w_HandleWindowMessages, this->m_Window).detach();

	this->m_Renderer = new Renderer();
	this->m_Renderer->r_SetWindowDimensions(this->m_Window->w_GetWindowDimensions());
	if (!this->m_Renderer->Initialize(this->m_Window->GetOverlayHWND())) {
		Utility::PrintError("Failed to Intiailize DirectX, Check installation");
		return false;
	}

	this->m_Game = new Game();
	if (!this->m_Game->Initialize()) {
		Utility::PrintError("Game not running, please open then run again");
		return false;
	}
	this->m_Game->SetScreenDimensions(this->m_Window->w_GetWindowDimensions());

	return true;
}

void App::Run() {
	std::thread RendererThread(&Renderer::Run, this->m_Renderer);
	std::thread GameThread(&Game::Run, this->m_Game);
	RendererThread.detach();
	GameThread.detach();

	while (true) {
		this->m_Renderer->r_SetViewMatrix(this->m_Game->GetViewMatrix());
		this->m_Renderer->r_SetEntities(this->m_Game->GetEntities(), this->m_Game->GetLocalEnt());
	}
}

void App::Shutdown() {

}