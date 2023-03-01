#include "patch_memory.h"

#include <Windows.h>
#include <iostream>

using namespace std;

int mem_patch(HANDLE proc, LPVOID addr, int val, int size) {
  DWORD old;
  
  VirtualProtectEx(proc, addr, size, PAGE_EXECUTE_READWRITE, &old);
  int result = WriteProcessMemory(proc, addr, &val, size, 0);
  VirtualProtectEx(proc, addr, size, old, &old);

  return result;
}