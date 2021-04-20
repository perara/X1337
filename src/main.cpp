
#include "../include/GameEngine.h"
//#include <windows.h>
int main()
{

//#if !_DEBUG
//	HWND hwnd_win = GetForegroundWindow();
//	ShowWindow(hwnd_win, SW_HIDE);
//#endif

	GameEngine g;

    // Start Gameloop
    g.runGame();
	return 0;
}
