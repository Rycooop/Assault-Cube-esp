#ifndef AC_WEAPON_H
#define AC_WEAPON_H

#include <iostream>
#include <cstdint>
#include <string>
#include <Windows.h>

#define WEAPONSETTINGS_OFFSET 0x104


struct WeaponClass {
	int16_t N00000C4B; //0x0000
	int16_t N00000EBE; //0x0002
	int16_t ReloadSpeedMs; //0x0004
	int16_t FireDelayMs; //0x0006
	int16_t Damage; //0x0008
	int16_t N00000EC2; //0x000A
	int16_t N00000D20; //0x000C
	int16_t N00000EC4; //0x000E
	int16_t Spread; //0x0010
	int16_t Knockback; //0x0012
	int16_t Magsize; //0x0014
	int16_t N00000EC8; //0x0016
	int16_t N00000C4E; //0x0018
	int16_t N00000ECA; //0x001A
	int16_t RecoilMomentumUp; //0x001C
	int16_t Recoil; //0x001E
	int16_t DropCrosshairAfterRecoil; //0x0020
	int16_t N00000ECE; //0x0022
	int8_t bCanRapidFire; //0x0024
};


struct WeaponSettings {
	WeaponSettings() = default;

	std::int16_t WeaponAmmo = 200;
	std::int16_t ReserveAmmo = 1000;

	std::int16_t ReloadSpeedMS = 0;
	std::int16_t FireDelayMS = 0;
	std::int16_t GunDamage = 1000;
	std::int16_t Spread = 0;
	std::int16_t Recoil = 0;
	std::int16_t WeaponKnockback = 0;
	std::int16_t MagSize = 200;
	std::int16_t RecoilMomentumVertical = 0;
	std::int16_t DropCrosshairAfterRecoil = 0;

	bool CanRapidFire = 1;
};

class Weapon {
public:
	Weapon() = default;
	Weapon(const HANDLE& pHandle, const std::uintptr_t Addr, const std::string& WeaponOwner);

	bool Initialize();

	bool UpdateWeaponSettings(WeaponSettings& Ws);
	WeaponSettings& GetCurrentSettings() { return *this->CurrSettings; }
	void RestoreWeaponSettings();

	inline std::string GetWeaponName() { return this->WeaponName; }
	inline std::string GetWeaponOwner() { return this->WeaponOwner; }

private:
	HANDLE pHandle;
	std::uintptr_t Addr;

	std::uintptr_t WeaponOwnerAddr;
	std::uintptr_t WeaponClassAddr;
	std::uintptr_t ReserveAmmoAddr;
	std::uintptr_t CurrAmmoAddr;

	const static std::size_t WeaponClassSize = 0x25;
	WeaponSettings* OriginalSettings;
	WeaponSettings* CurrSettings;

	std::string WeaponName;
	std::string WeaponOwner;
};



#endif //AC_WEAPON_H