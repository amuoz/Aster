#pragma once

#include "Game.h"

#define CONFIG_FILE PROJECT_SOURCE_DIR "/Aster/Config/config.conf"

static const std::string SRC_WIDTH = "SRC_WIDTH";
static const std::string SRC_HEIGHT = "SRC_HEIGHT";

class Physics;

extern std::unique_ptr<Game> g_game;
extern Physics* g_PhysicsPtr;
