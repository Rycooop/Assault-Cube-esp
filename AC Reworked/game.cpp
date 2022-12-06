#include "game.h"


bool Game::Initialize() {
	this->hProcess = Utility::GetHandleByProcessName(L"ac_client");
	if (this->hProcess == INVALID_HANDLE_VALUE) {
		Utility::PrintError("Failed to obtain handle to the game");
		return false;
	}

	this->ModuleBase = Utility::GetModuleBase(L"ac_client.exe", GetProcessId(this->hProcess));
	if (!this->ModuleBase) {
		Utility::PrintError("Failed to obtain module base");
		return false;
	}

	ReadProcessMemory(this->hProcess, (LPVOID)(this->ModuleBase + OFFSET_LOCALPLAYER), &this->LocalPlayerAddr, sizeof(std::uintptr_t), NULL);
	ReadProcessMemory(this->hProcess, (LPVOID)(this->ModuleBase + OFFSET_ENTITYLIST), &this->EntityListAddr, sizeof(std::uintptr_t), NULL);
	
	this->m_Mutex = new std::mutex();

	this->ViewMatrixAddr = this->ModuleBase + OFFSET_VIEWMATRIX;
	this->ScreenDimensions = Vector2();
	this->ViewMatrix = new std::vector<float>(16);

	this->LocalEnt = new Entity();
	ReadProcessMemory(this->hProcess, (LPVOID)(this->LocalPlayerAddr), this->LocalEnt, sizeof(Entity), NULL);

	std::uintptr_t WeaponClassAddr;
	ReadProcessMemory(this->hProcess, (LPVOID)(this->LocalPlayerAddr + 0x370), &WeaponClassAddr, sizeof(std::uintptr_t), NULL);

	this->LocalEnt->weapon = new Weapon(this->hProcess, WeaponClassAddr, this->LocalEnt->GetName());
	this->LocalEnt->weapon->Initialize();

	this->EntityPtrs = new std::vector<std::uintptr_t>(32);
	this->Ents = new std::vector<Entity*>(32);

	for (auto& curr : *this->Ents) {
		curr = new Entity();
	}

	return true;
}

void Game::Run() {

	while (true) {
		ReadProcessMemory(this->hProcess, (LPVOID)(this->LocalPlayerAddr), this->LocalEnt, sizeof(Entity), NULL);
		ReadProcessMemory(this->hProcess, (LPVOID)(this->ViewMatrixAddr), this->ViewMatrix->data(), 16 * sizeof(float), NULL);

		this->UpdateEntities();

		std::this_thread::sleep_for(std::chrono::microseconds(500)); //Solves Flickering
	}
}

std::vector<Entity*>* Game::GetEntities() {
	std::lock_guard<std::mutex> Lock(*this->m_Mutex); 
	return this->Ents;
}

void Game::UpdateEntities() {
	ReadProcessMemory(this->hProcess, (LPVOID)(this->ModuleBase + OFFSET_PLAYERCOUNT), &this->CurrentPlayerCount, sizeof(std::uint8_t), NULL);
	ReadProcessMemory(this->hProcess, (LPVOID)(this->EntityListAddr), this->EntityPtrs->data(), this->CurrentPlayerCount * 0x4, NULL);

	for (int i = 1; i < this->CurrentPlayerCount; i++) {
		ReadProcessMemory(this->hProcess, (LPVOID)(this->EntityPtrs->at(i)), &*this->Ents->at(i), sizeof(Entity), NULL);

		if (Utility::WorldToScreen(this->Ents->at(i)->EntityPos, *this->ViewMatrix, this->ScreenDimensions, this->Ents->at(i)->EntityBodyScreen) && Utility::WorldToScreen(this->Ents->at(i)->EntityHeadPos, *this->ViewMatrix, this->ScreenDimensions, this->Ents->at(i)->EntityHeadScreen)) {
			this->Ents->at(i)->IsVisible = true;
		}
		else this->Ents->at(i)->IsVisible = false;
	}

	std::fill(this->EntityPtrs->begin(), this->EntityPtrs->end(), 0);
}