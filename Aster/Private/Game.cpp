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

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>

// externs
Physics *g_PhysicsPtr;
//Config* g_Config;

__inline float Randf(float min, float max)
{
	return (float)((float)((rand() & 32767) * (1.0 / 32767.0)) * (max - min) + min);
}

Game::Game() : State(GameState::GAME_ACTIVE), Keys(), KeysProcessed()
{
}

Game::~Game()
{
	glfwTerminate();

	delete m_camera;
}

Player *Game::CreatePlayer(glm::vec3 playerPosition)
{
	const glm::vec3 PLAYER_SIZE(16.0f, 9.0f, 0.0f);
	glm::vec3 charScale(1.0f, 1.0f, 1.0f);
	charScale.x = Config::Get()->GetValue(SRC_WIDTH) / PLAYER_SIZE.x;
	charScale.y = Config::Get()->GetValue(SRC_HEIGHT) / PLAYER_SIZE.y;

	Sprite *playerSprite = new Sprite("player");

	playerSprite->AddAnimation("player_walk", AnimationType::WALK, 0.03f);
	playerSprite->AddAnimation("player_idle", AnimationType::IDLE, 0.06f);
	playerSprite->AddAnimation("attack_up", AnimationType::ATTACK_UP, 0.03f);
	playerSprite->AddAnimation("attack_right", AnimationType::ATTACK_RIGHT, 0.03f);
	playerSprite->AddAnimation("attack_down", AnimationType::ATTACK_DOWN, 0.03f);
	playerSprite->AddAnimation("attack_left", AnimationType::ATTACK_LEFT, 0.03f);

	Player *player = new Player(playerPosition, charScale, playerSprite);

	m_scene.push_back(player);

	return player;
}

void Game::InitGame()
{
	g_PhysicsPtr = new Physics(glm::vec3(0.0f, 0.0f, 0.0f));
	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	// text renderer with freetype
	m_text = new TextRenderer(Config::Get()->GetValue(SRC_WIDTH),
														Config::Get()->GetValue(SRC_HEIGHT));
	m_text->Load(PROJECT_SOURCE_DIR "/Aster/Fonts/arial.ttf", 24);

	m_gameTime = 0.0f;
	m_score = 0;

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
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/block.png", false, "block");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/block_solid.png", false, "block_solid");
	ResourceManager::GetInstance()->LoadTexture(PROJECT_SOURCE_DIR "/Aster/Textures/grass-background.png", true, "background");

	// level
	CurrentLevel = ResourceManager::GetInstance()->LoadLevel(PROJECT_SOURCE_DIR "/Aster/Levels/one.json", "one");
	glm::vec3 playerPosition = CurrentLevel->GetPlayerPosition();

	// Player
	Character = CreatePlayer(playerPosition);
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
		m_currentBulletFreq += deltaTime;
		m_gameTime += deltaTime;

		glm::vec4 playerAttackHitbox = Character->GetAttackHitbox();

		// ..:: LOGIC ::..
		for (std::list<Actor *>::iterator it = m_scene.begin(); it != m_scene.end();)
		{
			Actor *actor = (*it);
			if (actor->IsDestroyed)
			{
				actor->GetActorCollider()->active = false;
				it = m_scene.erase(it);
				delete actor;
			}
			else
			{
				actor->Update(deltaTime, playerAttackHitbox);
				it++;
			}
		}

		if (!Character->IsActive())
		{
			SetGameState(GameState::GAME_OVER);
		}

		// camera/view transformation
		glm::vec3 cameraPos(Character->GetPosition().x - Config::Get()->GetValue(SRC_WIDTH) / 2,
												Character->GetPosition().y - Config::Get()->GetValue(SRC_HEIGHT) / 2,
												0.0f);
		m_camera->SetPosition(cameraPos);

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
	Renderer->SetViewMatrix(m_camera->GetViewMatrix());

	// Draw background
	Texture2D background = ResourceManager::GetInstance()->GetTexture("background");
	Renderer->DrawTexture(background,
												glm::vec2(0.0f, 0.0f), glm::vec2(Config::Get()->GetValue(SRC_WIDTH), Config::Get()->GetValue(SRC_HEIGHT)), 0.0f);

	// DebugAttackHitbox(*Renderer);

	// Draw level
	CurrentLevel->Draw(*Renderer, deltaTime);

	// Render scene
	for (Actor *actor : m_scene)
	{
		if (actor->IsActive())
		{
			actor->Draw(*Renderer, deltaTime);
		}
	}

	// Render UI
	RenderUI();
}

void Game::RenderUI()
{
	float screenWidth = Config::Get()->GetValue(SRC_WIDTH);
	float screenHeight = Config::Get()->GetValue(SRC_HEIGHT);

	if (State == GameState::GAME_OVER)
	{
		m_text->RenderText("YOU ARE DEAD", 50, screenHeight / 2, 5.0, glm::vec3(0.7, 0.0, 0.0));
	}
	else
	{
		m_text->RenderText("Kill the Mobs", 650, Config::Get()->GetValue(SRC_HEIGHT) / 10, 1.0, glm::vec3(1.0, 1.0, 1.0));
	}
}

void Game::Restart()
{
	// hot-reload config file
	Config::Get()->Load(CONFIG_FILE);

	for (std::list<Actor *>::iterator it = m_scene.begin(); it != m_scene.end();)
	{
		Actor *actor = (*it);
		if (actor->IsDelete()) // clean runtime deleteable actors
		{
			actor->GetActorCollider()->active = false;
			it = m_scene.erase(it);
			delete actor;
		}
		else
		{
			actor->Reset();
			it++;
		}
	}

	m_gameTime = 0.0f;
	m_score = 0;
	SetGameState(GameState::GAME_ACTIVE);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::ProcessInput(float deltaTime)
{
	// Player movement
	if (State == GameState::GAME_ACTIVE)
	{
		bool bMove = false;
		glm::vec3 direction(0.0f);
		if (Keys[GLFW_KEY_A])
		{
			direction += glm::vec3(-1.0f, 0.0f, 0.0f);
			bMove = true;
		}
		if (Keys[GLFW_KEY_D])
		{
			direction += glm::vec3(1.0f, 0.0f, 0.0f);
			bMove = true;
		}
		if (Keys[GLFW_KEY_W])
		{
			direction += glm::vec3(0.0f, -1.0f, 0.0f);
			bMove = true;
		}
		if (Keys[GLFW_KEY_S])
		{
			direction += glm::vec3(0.0f, 1.0f, 0.0f);
			bMove = true;
		}

		// apply character movement if needed
		if (bMove)
		{
			Character->Move(deltaTime, direction);
		}
		else
		{
			Character->Idle();
		}

		// attack
		if (Keys[GLFW_KEY_SPACE])
		{
			Character->Attack();
		}
	}

	if (State == GameState::GAME_RESTART)
	{
		if (Keys[GLFW_KEY_R] && !KeysProcessed[GLFW_KEY_R])
		{
			std::cout << "...RESTART GAME..." << std::endl;
			KeysProcessed[GLFW_KEY_R] = true;
			Restart();
		}
	}
}

void Game::SetGameState(GameState newState)
{
	State = newState;
}

void Game::DebugAttackHitbox(SpriteRenderer &Renderer)
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec4 playerAttackHitbox = Character->GetAttackHitbox();
	Texture2D texture = ResourceManager::GetInstance()->GetTexture("block");
	Renderer.DrawTexture(
			texture,
			glm::vec2(playerAttackHitbox.x, playerAttackHitbox.y),
			glm::vec2(playerAttackHitbox.z, playerAttackHitbox.w),
			0.0f,
			glm::vec3(0.8f, 0.0f, 0.2f));
}
