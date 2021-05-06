/*****************************************************************
** Asteroids is a simple spaceship in asteroid field game.
** Developed with custom C++ engine.
** OpenGL3,	GLFW, GLAD, GLM, FreeType, VS2017
** Custom physics and collisions
** Generic entity pool, abstract Actor class
** Custom vertex shader, geometry shader and fragment shader
******************************************************************/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <list>
#include <map>
#include <utility>

// Forwards
class Physics;
class Camera;
class AsteroidMgr;
class Actor;
class TextRenderer;
class SpriteRenderer;
class Level;
class Player;
class PlayerController;
class GLFWwindow;
enum class GameState;

class Game
{
public:
	Game();
	virtual ~Game();

	void Execute(float deltaTime);

	void SetGameState(GameState newState);

	void InitGame(GLFWwindow *window);

private:
	std::shared_ptr<Player> CreatePlayer(glm::vec3 playerPosition);

	void ProcessInput(float deltaTime);
	void Update(float deltaTime); // logic and physics
	void Render(float deltaTime);
	void RenderUI();
	// void RemoveFromLevel(std::shared_ptr<Actor> actor);

	void Restart();

	void DebugAttackHitbox(SpriteRenderer &Renderer);

	Camera *m_camera;
	AsteroidMgr *m_AsteroidMgr;
	TextRenderer *m_text;

	float m_gameTime = 0.0f;

	std::shared_ptr<Level> CurrentLevel;
	PlayerController *CharacterController;

	GameState State;

	SpriteRenderer *Renderer;
};
