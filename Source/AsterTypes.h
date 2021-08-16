#pragma once

enum class GameState {
	GAME_ACTIVE,
	GAME_OVER
};

enum class AnimationType {
	WALK,
	WALK_UP,
	WALK_RIGHT,
	WALK_DOWN,
	WALK_LEFT,
	IDLE,
	IDLE_UP,
	IDLE_RIGHT,
	IDLE_DOWN,
	IDLE_LEFT,
	SWORD_UP,
	SWORD_RIGHT,
	SWORD_DOWN,
	SWORD_LEFT,
	SPEAR_UP,
	SPEAR_RIGHT,
	SPEAR_DOWN,
	SPEAR_LEFT,
	SHOOT_UP,
	SHOOT_RIGHT,
	SHOOT_DOWN,
	SHOOT_LEFT,
	HAMMER_RIGHT,
	HAMMER_LEFT,
	DASH_UP,
	DASH_RIGHT,
	DASH_DOWN,
	DASH_LEFT,
};

enum class ActorState {
	IDLE,
	MOVEMENT_UP,
	MOVEMENT_RIGHT,
	MOVEMENT_DOWN,
	MOVEMENT_LEFT,
	DASH_UP,
	DASH_RIGHT,
	DASH_DOWN,
	DASH_LEFT,
	ATTACK_UP,
	ATTACK_RIGHT,
	ATTACK_DOWN,
	ATTACK_LEFT,
	AGGRO
};

enum class PowerUpType {
  NONE,
  SWORD,
  SPEAR,
  HAMMER
};

enum class BlockLocation {
		TOP_LEFT, 		TOP, 			TOP_RIGHT,
		LEFT, 				MIDDLE, 	RIGHT,
		BOTTOM_LEFT, 	BOTTOM, 	BOTTOM_RIGHT,

		FLOOR,
	};

enum class CollisionChannel {
	DYNAMIC,
	STATIC,
	PLAYER
};

enum class CollisionResponse {
	IGNORED,
	OVERLAP,
	BLOCK
};

enum class ActorType {
	NONE,
	DESTROYABLE_BLOCK,
	BLOCK,
	DOOR,
	FLOOR
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
