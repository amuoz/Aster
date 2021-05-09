#pragma once

enum class GameState {
	GAME_ACTIVE,
	GAME_OVER
};

enum class AnimationType {
	WALK,
	IDLE,
	SWORD_UP,
	SWORD_RIGHT,
	SWORD_DOWN,
	SWORD_LEFT,
    SPEAR_UP,
    SPEAR_RIGHT,
    SPEAR_DOWN,
    SPEAR_LEFT,
	DASH_RIGHT,
	DASH_LEFT
};

enum class ActorState {
    IDLE,
    MOVEMENT_UP,
    MOVEMENT_RIGHT,
    MOVEMENT_DOWN,
    MOVEMENT_LEFT,
    DASH,
	ATTACK_UP,
	ATTACK_RIGHT,
	ATTACK_DOWN,
	ATTACK_LEFT,
    AGGRO
};

enum class PowerUpType {
  NONE,
  SWORD,
  SPEAR
};

namespace Aster {
	struct Rectangle
	{
		unsigned int vertexArrayObject;
		unsigned int vertexBufferObject;
		unsigned int colorBufferObject;
		unsigned int uvBufferObject;

		void Draw();
	};
}