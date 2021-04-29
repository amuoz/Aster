#pragma once

enum class GameState {
	GAME_ACTIVE,
	GAME_RESTART,
  GAME_OVER
};

enum class AnimationType {
    WALK,
    IDLE,
    ATTACK_UP,
    ATTACK_RIGHT,
    ATTACK_DOWN,
    ATTACK_LEFT,
};

enum class ActorState {
    IDLE,
    MOVEMENT_UP,
    MOVEMENT_RIGHT,
    MOVEMENT_DOWN,
    MOVEMENT_LEFT,
		ATTACK_UP,
		ATTACK_RIGHT,
		ATTACK_DOWN,
		ATTACK_LEFT,
};

struct Rectangle
{
  unsigned int vertexArrayObject;
  unsigned int vertexBufferObject;
  unsigned int colorBufferObject;
  unsigned int uvBufferObject;

  void Draw();
};
