#include "Common.h"
#include "Game.h"

Game* g_game;

int main(int argc, char *argv[])
{
	std::cout << "==================== ...INIT GAME... =======================" << std::endl;
	g_game = new Game();
	std::cout << "==================== GAME INITIALIZED =======================" << std::endl;

	// game loop
	while (!g_game->IsFinished())
	{
		g_game->Execute();
	}

	// free resources
	delete g_game;

	return 0;
}
