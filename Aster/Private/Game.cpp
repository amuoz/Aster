#include "Game.h"

#include "Common.h"
#include "Physics.h"
#include "Shader.h"
#include "Camera.h"
#include "Config.h"
#include "Level.h"
#include "Player.h"
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

std::shared_ptr<Player> Game::CreatePlayer(glm::vec3 playerPosition)
{
	const glm::vec3 PLAYER_SIZE(16.0f, 9.0f, 0.0f);
	glm::vec3 charScale(1.0f, 1.0f, 1.0f);
	charScale.x = Config::Get()->GetValue(SRC_WIDTH) / PLAYER_SIZE.x;
	charScale.y = Config::Get()->GetValue(SRC_HEIGHT) / PLAYER_SIZE.y;

	Sprite *playerSprite = new Sprite("player");

	playerSprite->AddAnimation("player_walk", AnimationType::WALK, 0.03f);
	playerSprite->AddAnimation("player_idle", AnimationType::IDLE, 0.06f);
	playerSprite->AddAnimation("spear_up", AnimationType::SPEAR_UP, 0.03f);
	playerSprite->AddAnimation("spear_right", AnimationType::SPEAR_RIGHT, 0.03f);
	playerSprite->AddAnimation("spear_down", AnimationType::SPEAR_DOWN, 0.03f);
	playerSprite->AddAnimation("spear_left", AnimationType::SPEAR_LEFT, 0.03f);
	playerSprite->AddAnimation("sword_up", AnimationType::SWORD_UP, 0.03f);
	playerSprite->AddAnimation("sword_right", AnimationType::SWORD_RIGHT, 0.03f);
	playerSprite->AddAnimation("sword_down", AnimationType::SWORD_DOWN, 0.03f);
	playerSprite->AddAnimation("sword_left", AnimationType::SWORD_LEFT, 0.03f);
	playerSprite->AddAnimation("roll_right", AnimationType::DASH_RIGHT, 0.018f);
	playerSprite->AddAnimation("roll_left", AnimationType::DASH_LEFT, 0.018f);

	std::shared_ptr<Player> player = std::make_shared<Player>(
			playerPosition, charScale, playerSprite);

	return player;
}

void Game::InitGame(GLFWwindow *window)
{
	g_PhysicsPtr = new Physics(glm::vec3(0.0f, 0.0f, 0.0f));
	PlayerCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	// text renderer with freetype
	Text = new TextRenderer(Config::Get()->GetValue(SRC_WIDTH),
														Config::Get()->GetValue(SRC_HEIGHT));
	Text->Load(PROJECT_SOURCE_DIR "/Aster/Fonts/arial.ttf", 24);

	m_gameTime = 0.0f;

	// Initialize sprite renderer with sprite shader
	ResourceManager::GetInstance()->LoadShader(PROJECT_SOURCE_DIR "/Aster/Shaders/sprite.vs", PROJECT_SOURCE_DIR "/Aster/Shaders/sprite.fs", nullptr, "sprite");
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
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/player.png", true, "player");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/spike_enemy.png", true, "spike_enemy");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/bone.png", true, "spear_powerup");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/anvil.png", true, "sword_powerup");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/block.png", false, "block");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/block_solid.png", false, "block_solid");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/grass-background.png", true, "background");

	// level
	CurrentLevel = ResourceManager::GetInstance()->LoadLevel(PROJECT_SOURCE_DIR "/Aster/Levels/one.json", "one");
	glm::vec3 playerPosition = CurrentLevel->GetPlayerPosition();

	// Player
	std::shared_ptr<Player> player = CreatePlayer(playerPosition);
	CharacterController = new PlayerController(player, window, Renderer, Text);
	CurrentLevel->AddPlayer(player);
}

void Game::Execute(float deltaTime)
{
	if (State != GameState::GAME_OVER)
		this->ProcessInput(deltaTime);

	this->Update(deltaTime);

	this->Render(deltaTime);
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
	glClear(GL_COLOR_BUFFER_BIT);

	// Set camera view matrix
	Renderer->SetViewMatrix(PlayerCamera->GetViewMatrix());

	// Draw background
	Texture2D background = ResourceManager::GetInstance()->GetTexture("background");
	Renderer->DrawTexture(background,
												glm::vec2(0.0f, 0.0f), glm::vec2(Config::Get()->GetValue(SRC_WIDTH), Config::Get()->GetValue(SRC_HEIGHT)), 0.0f);

	// DebugAttackHitbox(*Renderer);

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

	CurrentLevel->Reset();

	m_gameTime = 0.0f;
	SetGameState(GameState::GAME_ACTIVE);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::ProcessInput(float deltaTime)
{
	// Player movement
	if (State == GameState::GAME_ACTIVE)
	{
		CharacterController->ProcessInput(deltaTime);
	}

	// if (State == GameState::GAME_RESTART)
	// {
	// 	if (Keys[GLFW_KEY_R] && !KeysProcessed[GLFW_KEY_R])
	// 	{
	// 		std::cout << "...RESTART GAME..." << std::endl;
	// 		KeysProcessed[GLFW_KEY_R] = true;
	// 		Restart();
	// 	}
	// }
}

void Game::SetGameState(GameState newState)
{
	State = newState;
}

void Game::DebugAttackHitbox(SpriteRenderer &Renderer)
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec4 playerAttackHitbox = CharacterController->GetAttackHitbox();
	Texture2D texture = ResourceManager::GetInstance()->GetTexture("block");
	Renderer.DrawTexture(
			texture,
			glm::vec2(playerAttackHitbox.x, playerAttackHitbox.y),
			glm::vec2(playerAttackHitbox.z, playerAttackHitbox.w),
			0.0f,
			glm::vec3(0.8f, 0.0f, 0.2f));
}
