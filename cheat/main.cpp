#define _WIN32_WINNT 0x0502

#include <Windows.h>
#include <iostream>
#include "patches/ammo.h"

using namespace std;

DWORD WINAPI MainThread(LPVOID param) {
  while (true) {
    if (GetAsyncKeyState(VK_F1) & 0x80000) {
      // This will activate our cheat
      cout << '\a';
      MessageBoxW(NULL, L"Infinite Ammo Activated!", L"Test", MB_OK);
    }

    refillAmmo(GetCurrentProcess());

    Sleep(100);
  }

  return 0;
}

bool WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
  if (dwReason == DLL_PROCESS_ATTACH) {
    MessageBoxW(NULL, L"DLL Injected!", L"Test", MB_OK);
    CreateThread(0, 0, MainThread, hModule, 0, 0);
  }

  return true;
}