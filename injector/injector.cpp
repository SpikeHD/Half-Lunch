#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

// Basically everything done here was learned through tis video :)
// https://www.youtube.com/watch?v=KCtLiBnlpk4

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

int main(int argc, char* argv[]) {
  const wchar_t *proc_name = L"hl.exe";
  int pid = getProcID(proc_name);
  DWORD e;

  cout << "Found PID: " << pid << endl;

  if (pid == 0) {
    MessageBoxW(NULL, L"PID could not be found. Is the game running?", L"Error finding process ID", MB_OK);
    return -1;
  }

  char dll[] = "half_lunch.dll";
  char path[MAX_PATH] = {0};

  GetFullPathNameA(dll, MAX_PATH, path, NULL);

  size_t pathlen = strlen(path) + 1;

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

  if (hProcess == NULL) {
    cout << "Injection failed: could not find process" << endl;
    return -1;
  }

  LPVOID pszLibFileRemote = VirtualAllocEx(hProcess, NULL, pathlen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

  if (pszLibFileRemote == NULL) {
    cout << "Injection failed: virtual allocation failure" << endl;
    return -1;
  }

  WriteProcessMemory(hProcess, pszLibFileRemote, path, pathlen, NULL);
  HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, pszLibFileRemote, NULL, NULL);

  if (hThread == NULL) {
    cout << "Injection failed: thread exited with error" << endl;
    return -1;
  }

  WaitForSingleObject(hThread, INFINITE);

  GetExitCodeThread(hThread, &e);

  cout << "Process exited with code: " << e << endl;

  CloseHandle(hThread);
  VirtualFreeEx(hProcess, path, 0, MEM_RELEASE);
  CloseHandle(hProcess);

  return 0;
}
