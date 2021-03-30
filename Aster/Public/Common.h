#pragma once

#include "Game.h"

#define CONFIG_FILE PROJECT_SOURCE_DIR "/Aster/Config/config.conf"

//class Game;
class Physics;

extern std::unique_ptr<Game> g_game;
extern Physics* g_PhysicsPtr;
