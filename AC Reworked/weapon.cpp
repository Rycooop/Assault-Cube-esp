#include "weapon.h"


Weapon::Weapon(const HANDLE& pHandle, const std::uintptr_t Addr, const std::string& WeaponOwner) {
	this->pHandle = pHandle;
	this->Addr = Addr;
	this->WeaponOwner = WeaponOwner;

	this->OriginalSettings = new WeaponSettings();
	this->CurrSettings = new WeaponSettings();
}

bool Weapon::Initialize() {
	ReadProcessMemory(this->pHandle, (LPVOID)(this->Addr + 0x8), &this->WeaponOwnerAddr, sizeof(std::uintptr_t), NULL);
	ReadProcessMemory(this->pHandle, (LPVOID)(this->Addr + 0xc), &this->WeaponClassAddr, sizeof(std::uintptr_t), NULL);
	ReadProcessMemory(this->pHandle, (LPVOID)(this->Addr + 0x10), &this->ReserveAmmoAddr, sizeof(std::uintptr_t), NULL);
	ReadProcessMemory(this->pHandle, (LPVOID)(this->Addr + 0x14), &this->CurrAmmoAddr, sizeof(std::uintptr_t), NULL);

	std::string wName(16, '\0');
	ReadProcessMemory(this->pHandle, (LPVOID)this->WeaponClassAddr, (LPVOID)wName.data(), 16, NULL);

	if (wName.length()) {
		this->WeaponName = wName;
	}
	else this->WeaponName = "Unknown Weapon";

	WeaponClass SettingsOrig;
	ReadProcessMemory(this->pHandle, (LPVOID)(this->WeaponClassAddr + WEAPONSETTINGS_OFFSET), &SettingsOrig, sizeof(SettingsOrig), NULL);

	ZeroMemory(this->OriginalSettings, sizeof(WeaponSettings));
	this->OriginalSettings->WeaponAmmo = SettingsOrig.Magsize;
	this->OriginalSettings->ReloadSpeedMS = SettingsOrig.ReloadSpeedMs;
	this->OriginalSettings->FireDelayMS = SettingsOrig.FireDelayMs;
	this->OriginalSettings->GunDamage = SettingsOrig.Damage;
	this->OriginalSettings->Spread = SettingsOrig.Spread;
	this->OriginalSettings->Recoil = SettingsOrig.Recoil;
	this->OriginalSettings->WeaponKnockback = SettingsOrig.Knockback;
	this->OriginalSettings->MagSize = SettingsOrig.Magsize;
	this->OriginalSettings->RecoilMomentumVertical = SettingsOrig.RecoilMomentumUp;
	this->OriginalSettings->DropCrosshairAfterRecoil = SettingsOrig.DropCrosshairAfterRecoil;
	ReadProcessMemory(this->pHandle, (LPVOID)this->ReserveAmmoAddr, &this->OriginalSettings->ReserveAmmo, sizeof(std::int16_t), NULL);

	this->CurrSettings = this->OriginalSettings;

	return true;
}

bool Weapon::UpdateWeaponSettings(WeaponSettings& Ws) {
	this->CurrSettings = &Ws;

	WeaponClass wc;
	ZeroMemory(&wc, sizeof(WeaponClass));
	wc.bCanRapidFire =				this->CurrSettings->CanRapidFire;
	wc.Recoil =						this->CurrSettings->Recoil;
	wc.Spread =						this->CurrSettings->Spread;
	wc.FireDelayMs =				this->CurrSettings->FireDelayMS;
	wc.Damage =						this->CurrSettings->GunDamage;
	wc.DropCrosshairAfterRecoil =	this->CurrSettings->DropCrosshairAfterRecoil;
	wc.Knockback =					this->CurrSettings->WeaponKnockback;
	wc.RecoilMomentumUp =			this->CurrSettings->RecoilMomentumVertical;
	wc.Magsize =					this->CurrSettings->MagSize;

	return WriteProcessMemory(this->pHandle, (LPVOID)(this->WeaponClassAddr + WEAPONSETTINGS_OFFSET), &wc, sizeof(wc), NULL);
}

void Weapon::RestoreWeaponSettings() {
	WeaponClass wc;
	ZeroMemory(&wc, sizeof(WeaponClass));
	wc.bCanRapidFire =					this->OriginalSettings->CanRapidFire;
	wc.Recoil =							this->OriginalSettings->Recoil;
	wc.Spread =							this->OriginalSettings->Spread;
	wc.FireDelayMs =					this->OriginalSettings->FireDelayMS;
	wc.Damage =							this->OriginalSettings->GunDamage;
	wc.DropCrosshairAfterRecoil =		this->OriginalSettings->DropCrosshairAfterRecoil;
	wc.Knockback =						this->OriginalSettings->WeaponKnockback;
	wc.RecoilMomentumUp =				this->OriginalSettings->RecoilMomentumVertical;
	wc.Magsize =						this->OriginalSettings->MagSize;

	WriteProcessMemory(this->pHandle, (LPVOID)(this->WeaponClassAddr + WEAPONSETTINGS_OFFSET), &wc, sizeof(wc), NULL);
}