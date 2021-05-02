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

// Forwards
class Physics;
class Camera;
class AsteroidMgr;
class Actor;
class TextRenderer;
class SpriteRenderer;
class Level;
class Player;
enum class GameState;

class Game
{
public:
	Game();
	virtual ~Game();

	void Execute(float deltaTime);

	int GetScore() { return m_score; }
	void IncreaseScore() { ++m_score; }

	void SetGameState(GameState newState);

	void InitGame();

	bool Keys[1024];
	bool KeysProcessed[1024];

private:
	
	void InitPlayer();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);	// logic and physics
	void Render(float deltaTime);
	
	void RenderUI();

	void Restart();

	void DebugAttackHitbox(SpriteRenderer& Renderer);

	GLFWwindow* m_window;
	
	Camera* m_camera;
	AsteroidMgr* m_AsteroidMgr;
	TextRenderer* m_text;
	
	float m_gameTime = 0.0f;
	float m_currentBulletFreq;

	// scene actors
	std::list<Actor*> m_scene;

	std::shared_ptr<Level> CurrentLevel;
	Player* Character;

	GameState State;

	int m_score;

	SpriteRenderer* Renderer;
};
