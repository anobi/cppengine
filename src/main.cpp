#include <iostream>
#include "configuration.hpp"
#include "game.hpp"



#ifdef _WIN32

#include <Windows.h>
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	) 
{
	Game game;
	game.Start();

	return 0;
}
#else

std::string GetWorkingDirectory(char* executable_path)
{
	std::string::size_type pos = std::string(executable_path).find_last_of( "\\/" );
    return std::string(executable_path).substr( 0, pos);
}

int main(int argc, char* argv[]) 
{
	Game game;

	char* exe = argv[0];
	Configuration::Get().workingDirectory = GetWorkingDirectory(exe);

	game.Start();

	std::cout << "* Exiting... Bye bye!" << std::endl;
	return EXIT_SUCCESS;
}
#endif
