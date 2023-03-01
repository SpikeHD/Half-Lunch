#include <Windows.h>
#include <iostream>
#include "ammo.h"
#include "patch_memory.h"

int refillAmmo(HANDLE hProcess) {
  return mem_patch(hProcess, (LPVOID)PISTOL_AMMO, 69, 4);
}

int getRealAmmoCount(HANDLE hProcess) {
  int ammo;
  ReadProcessMemory(hProcess, (LPCVOID)PISTOL_AMMO, &ammo, 4, 0);
  return ammo;
}