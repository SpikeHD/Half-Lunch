#include "patch_memory.h"

#include <Windows.h>

void mem_patch(HANDLE proc, LPVOID addr, int val, int size) {
  DWORD old;

  VirtualProtect(proc, size, PAGE_EXECUTE_READWRITE, &old);
  WriteProcessMemory(proc, addr, (LPCVOID)val, size, 0);
  VirtualProtect(proc, size, old, &old);
}