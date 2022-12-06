#ifndef AC_GAME_H
#define AC_GAME_H

#include <string>
#include <vector>
#include <mutex>
#include <thread>

#include "entity.h"
#include "utility.h"

#define OFFSET_PLAYERCOUNT	0x10F500
#define OFFSET_VIEWMATRIX	0x101AE8
#define OFFSET_LOCALPLAYER	0x10f4f4
#define OFFSET_ENTITYLIST	0x10f4f8


class Game {
public:
	Game() = default;
	~Game() = default;

	bool Initialize();
	void Run();

	inline std::vector<float>* GetViewMatrix() { return &*this->ViewMatrix; }
	std::vector<Entity*>* GetEntities();
	inline Entity* GetLocalEnt() { return this->LocalEnt; }

	inline void SetScreenDimensions(const Vector2 Dimensions) { this->ScreenDimensions = Dimensions; }

private:
	void UpdateEntities();

	std::mutex* m_Mutex;

	HANDLE hProcess;
	Vector2 ScreenDimensions;

	std::uintptr_t ModuleBase;
	std::uintptr_t LocalPlayerAddr;
	std::uintptr_t EntityListAddr;
	std::uintptr_t ViewMatrixAddr;

	std::int8_t CurrentPlayerCount;

	std::vector<float>* ViewMatrix;
	Entity* LocalEnt;
	std::vector<std::uintptr_t>* EntityPtrs;
	std::vector<Entity*>* Ents;

};

#endif //AC_GAME_H