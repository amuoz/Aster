#include "Game.h"

#include "Common.h"
#include "Physics.h"
#include "Shader.h"
#include "Camera.h"
#include "Config.h"
#include "Level.h"
#include "Entity/Player.h"
#include "Sprite.h"
#include "Animation.h"

#include "ResourceManager.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerController.h"

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <GLFW/glfw3.h>

// externs
Physics *g_PhysicsPtr;

__inline float Randf(float min, float max)
{
	return (float)((float)((rand() & 32767) * (1.0 / 32767.0)) * (max - min) + min);
}

Game::Game() : State(GameState::GAME_ACTIVE)
{
}

Game::~Game()
{
	glfwTerminate();

	delete PlayerCamera;
}

void Game::InitGame(GLFWwindow *window)
{
	g_PhysicsPtr = new Physics(glm::vec3(0.0f, 0.0f, 0.0f));
	PlayerCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	// text renderer with freetype
	Text = new TextRenderer(Config::Get()->GetValue(SRC_WIDTH),
													Config::Get()->GetValue(SRC_HEIGHT));
	Text->Load(PROJECT_SOURCE_DIR "/Fonts/arial.ttf", 24);

	m_gameTime = 0.0f;

	// Initialize sprite renderer with sprite shader
	ResourceManager::GetInstance()->LoadShader(PROJECT_SOURCE_DIR "/Shaders/sprite.vs", PROJECT_SOURCE_DIR "/Shaders/sprite.fs", nullptr, "sprite");
	ResourceManager::GetInstance()->GetShader("sprite").Use();
	ResourceManager::GetInstance()->GetShader("sprite").SetInteger("image", 0);
	glm::mat4 projection = glm::ortho(0.0f,
																		(float)Config::Get()->GetValue(SRC_WIDTH),
																		(float)Config::Get()->GetValue(SRC_HEIGHT),
																		0.0f, -1.0f, 1.0f);
	ResourceManager::GetInstance()->GetShader("sprite").SetMatrix4("projection", projection);
	auto shader = ResourceManager::GetInstance()->GetShader("sprite");
	Renderer = new SpriteRenderer(shader);

	// load textures
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/player.png", true, "player");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/spike_enemy.png", true, "spike_enemy");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/bone.png", true, "spear_powerup");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/anvil.png", true, "sword_powerup");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/rock.png", true, "hammer_powerup");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block.png", true, "block");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_bottom.png", true, "block_bottom");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_corner_left.png", true, "block_corner_left");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_corner_right.png", true, "block_corner_right");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_side_left.png", true, "block_side_left");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_side_right.png", true, "block_side_right");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_corner_top_right.png", true, "block_top_right");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_corner_top_left.png", true, "block_top_left");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_top.png", true, "block_top");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/block_solid.png", false, "block_solid");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Textures/grass-background.png", true, "background");

	// level
	CurrentLevel = ResourceManager::GetInstance()->LoadLevel(PROJECT_SOURCE_DIR "/Levels/one.json", "one");

	// Player controller
	CharacterController = new PlayerController(window, Renderer, Text);
	CharacterController->Posses(CurrentLevel->GetPlayer());
}

void Game::Execute(float deltaTime)
{
	ProcessInput(deltaTime);

	Update(deltaTime);

	Render(deltaTime);
}

void Game::Update(float deltaTime)
{
	if (State == GameState::GAME_ACTIVE)
	{
		m_gameTime += deltaTime;

		glm::vec4 playerAttackHitbox = CharacterController->GetAttackHitbox();

		if (!CharacterController->IsActive())
		{
			SetGameState(GameState::GAME_OVER);
		}

		// camera/view transformation
		glm::vec3 cameraPos(CharacterController->GetPosition().x - Config::Get()->GetValue(SRC_WIDTH) / 2,
												CharacterController->GetPosition().y - Config::Get()->GetValue(SRC_HEIGHT) / 2,
												0.0f);
		PlayerCamera->SetPosition(cameraPos);

		// Physics simulation
		g_PhysicsPtr->Update(deltaTime);

		CurrentLevel->Update(deltaTime, playerAttackHitbox);
	}
}

void Game::Render(float deltaTime)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set camera view matrix
	Renderer->SetViewMatrix(PlayerCamera->GetViewMatrix());

	// Draw background
	Texture2D background = ResourceManager::GetInstance()->GetTexture("background");
	Renderer->DrawTexture(background,
												glm::vec3(0.0f, Config::Get()->GetValue(LVL_HEIGHT), 0.0f),
												glm::vec2(Config::Get()->GetValue(LVL_WIDTH),
																	Config::Get()->GetValue(LVL_HEIGHT)),
												0.0f);

	DebugAttackHitbox(*Renderer);

	CurrentLevel->Draw(*Renderer, deltaTime);

	// Render UI
	if (State == GameState::GAME_OVER)
	{
		CharacterController->DrawPlayerDeath();
	}
	else
	{
		CharacterController->DrawObjectives();
	}

	CharacterController->DrawUI(PlayerCamera->GetPosition());
}

void Game::Restart()
{
	// hot-reload config file
	Config::Get()->Load(CONFIG_FILE);

	// level
	CurrentLevel = ResourceManager::GetInstance()->LoadLevel(PROJECT_SOURCE_DIR "/Levels/one.json", "one");
	CharacterController->Posses(CurrentLevel->GetPlayer());

	m_gameTime = 0.0f;
	SetGameState(GameState::GAME_ACTIVE);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::ProcessInput(float deltaTime)
{
	CharacterController->ProcessInput(deltaTime);
}

void Game::SetGameState(GameState newState)
{
	State = newState;
}

GameState Game::GetGameState()
{
	return State;
}

void Game::DebugAttackHitbox(SpriteRenderer &Renderer)
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec4 playerAttackHitbox = CharacterController->GetAttackHitbox();
	Texture2D texture = ResourceManager::GetInstance()->GetTexture("block");
	Renderer.DrawTexture(
			texture,
			glm::vec3(playerAttackHitbox.x, playerAttackHitbox.y, 9999.9f),
			glm::vec2(playerAttackHitbox.z, playerAttackHitbox.w),
			0.0f,
			glm::vec3(0.8f, 0.0f, 0.2f));
}
