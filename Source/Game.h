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
struct GLFWwindow;
enum class GameState;

class Game
{
public:
	Game();
	virtual ~Game();

	void Execute(float deltaTime);

	void SetGameState(GameState newState);
	GameState GetGameState();

	void InitGame(GLFWwindow *window);

	void Restart();

private:
	
	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render(float deltaTime);

	void DebugAttackHitbox(SpriteRenderer &Renderer);

	Camera *PlayerCamera;

	float m_gameTime = 0.0f;

	std::shared_ptr<Level> CurrentLevel;
	PlayerController* CharacterController;

	GameState State;

	SpriteRenderer *Renderer;
	TextRenderer *Text;
};
