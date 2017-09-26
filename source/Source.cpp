
#include <iostream>
#include "windows.h"
#include "direct.h"


using namespace std;

#define GCC_ERROR(str) do { (void)sizeof(str); } while(0) 

class gameCodeApp {

public:
	gameCodeApp getinstance() {
		return *instance;
	}
	LPCTSTR getgametitle() {
		return "Lab2";
	}
private:
	gameCodeApp* instance;
};
bool IsOnlyInstance(LPCTSTR gameTitle) {

	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);

	if (GetLastError() != ERROR_SUCCESS) {
		HWND hWnd = FindWindow(gameTitle, NULL);

		if (hWnd) {

			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);

			return false;
		}
	}
	return true;
}



bool CheckStorage(const DWORDLONG diskSpaceNeeded) {

	int const drive = _getdrive();
	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);
	unsigned __int64 const neededClusters = diskSpaceNeeded / (diskfree.sectors_per_cluster*diskfree.bytes_per_sector);
	
	if (diskfree.avail_clusters < neededClusters) {
		// if you get here you don¡¯t have enough disk space! 
		cout << "CheckStorage Failure : Not enough physical storage." << endl;

		return false;

	}

	return true;

}



DWORD ReadCPUSpeed() {

	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;


	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);



	if (lError == ERROR_SUCCESS){
		
		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}

	return dwMHz;

}



void checkMemory(int MB) {
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);
	cout << "the RAM available is: " << status.ullTotalPhys / MB << " MB." << endl;
	cout << "The Visual memory is: " << status.ullTotalVirtual / MB << " MB." << endl;
}


int main() {
	
		int MB = 1024 * 1024;
		gameCodeApp* a = new gameCodeApp();
		if (IsOnlyInstance(a->getgametitle())) {
			cout << "The game " <<a->getgametitle()<< " is already running" << endl;
		}
		else

		{
			cout << "The game is not running." << endl;
		}


		if (CheckStorage(300 * MB))
			cout << "There is enough disk space to run." << endl;
		else
			cout << "There isn't enough space to run." << endl;
		checkMemory(MB);
		cout << "CPU speed is: " << ReadCPUSpeed() << " GHz." << endl;
	system("pause");
	return 0;
}