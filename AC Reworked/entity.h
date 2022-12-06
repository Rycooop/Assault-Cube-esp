#ifndef AC_ENTITY_H
#define AC_ENTITY_H

#include <string>
#include <vector>
#include <mutex>

#include "vector.h"
#include "utility.h"
#include "weapon.h"


class Entity {
public:
	Entity() {
		this->EntityHeadPos = Vector3();
		this->EntityVelocity = Vector3();
		this->EntityPos = Vector3();
		this->EntityViewAngles = Vector3();
		this->Health = 0;
		this->Armor = 0;

		this->TeamNum = 0;
		this->IsDead = 1;

		this->IsVisible = false;
		this->EntityBodyScreen = Vector2();
		this->EntityHeadScreen = Vector2();

		this->Mutex = new std::mutex();
	}

	~Entity() = default;

	inline std::string GetTeam() { return this->TeamNum % 2 == 0 ? "CLA" : "RVSF"; } 
	inline std::string GetName() { return std::string(this->PlayerName); }

	std::uint32_t PlayerState;
	Vector3 EntityHeadPos;
	Vector3 EntityVelocity;
	char Unknown2[24];
	Vector3 EntityPos;
	Vector3 EntityViewAngles;
	char Unknown3[172];
	int Health;
	int Armor;
	char Unknown4[293];
	char PlayerName[16];
	char Unknown5[247];
	std::uint8_t TeamNum; //0x32C
	char Unknown6[11];
	std::uint8_t IsDead; //0x338 5 == SPEED

	//0x69 IsNotJumping

	Weapon* weapon;
	std::vector<Weapon*>* Weapons;

	bool IsVisible;
	Vector2 EntityBodyScreen;
	Vector2 EntityHeadScreen;

	std::mutex* Mutex;

};

#endif //AC_ENTITY_H