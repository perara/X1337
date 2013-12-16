#include "GameEngine.h"
#include <SFML\Audio.hpp>
#include<windows.h>
int main()
{

#if !DEBUG
	HWND hwnd_win = GetForegroundWindow();
	ShowWindow(hwnd_win, SW_HIDE);
#endif

	GameEngine* gaym = new GameEngine();

	return 0;
}
