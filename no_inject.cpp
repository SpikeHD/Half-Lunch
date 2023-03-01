#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <ctime>

#include "patch_memory.h"
#include "ammo.h"

using namespace std;

struct config {
  bool infinite_ammo = false;
};

int getProcID(const wchar_t *proc_name) {
  DWORD pid = 0;
  PROCESSENTRY32 pe32;

  pe32.dwSize = sizeof(PROCESSENTRY32);

  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if(Process32First(hSnapshot, &pe32) == 1)
  {
    while(Process32Next(hSnapshot, &pe32) == 1)
    {
      if(wcscmp(pe32.szExeFile, proc_name) == 0)
      {
        CloseHandle(hSnapshot);
        return pe32.th32ProcessID;
      }
    }
  }
}

string onOff(bool b) {
  return b ? "ON" : "OFF";
}

void drawUI(HANDLE hProcess, struct config cfg) {
  // Clear the screen
  system("cls");

  // Draw the UI
  cout << "Half Lunch: Half-Life 1 Cheat Thing" << endl;
  cout << endl;
  cout << "1: Infinite Ammo (Current ammo count: " << getRealAmmoCount(hProcess) << ") - [" << onOff(cfg.infinite_ammo) << "]" << endl;
}

int main() {
  std::time_t curTime = std::time(nullptr);
  std::time_t lastInput = curTime;
  bool didInput = false;

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, getProcID(L"hl.exe"));
  struct config cfg;

  if (hProcess == NULL) {
    cout << "Could not find process, is hl.exe open?" << endl;
    return -1;
  }

  // Initial draw
  drawUI(hProcess, cfg);

  while (true) {
    curTime = std::time(nullptr);
    
    // If lastInput was more than 1 second ago, allow another input
    if (curTime - lastInput > 1) {
      if (GetAsyncKeyState('1') & 0x80000) {
        // This will activate our cheat
        cout << '\a';
        drawUI(hProcess, cfg);

        cfg.infinite_ammo = !cfg.infinite_ammo;

        didInput = true;
      }

      if (didInput) {
        drawUI(hProcess, cfg);
        lastInput = curTime;
      }
    }

    if (cfg.infinite_ammo) refillAmmo(hProcess);

    Sleep(100);
  }

  return 0;
}