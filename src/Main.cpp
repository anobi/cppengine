#include "Game.hpp"
#include <Windows.h>



#ifdef _WIN32
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	) {
	Game game;
	game.Start();

	return 0;
}
#else
int main() {
	Game game;
	game.Start();

	return 0;
}
#endif