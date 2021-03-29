#include "Common.h"
#include "Game.h"

Game* g_game;

int main(int, char**)
{
	std::cout << "==================== ...INIT GAME... =======================" << std::endl;
	g_game = new Game();
	std::cout << "==================== GAME INITIALIZED =======================" << std::endl;

	if (g_game != nullptr)
	{
		// game loop
		while (!g_game->IsFinished())
		{
			g_game->Execute();
		}

		// free resources
		delete g_game;
	}
	else
	{
		std::cout << "ERROR: Something went wrong initializing Game" << std::endl;
	}


	return 0;
}
