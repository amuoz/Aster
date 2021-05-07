#include "PlayerController.h"

#include <utility>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Player.h"
#include "Config.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"

bool keys[1024];
bool keysProcessed[1024];

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

const unsigned int UI_PADDING = 10;
const glm::vec2 UI_ITEM_SIZE = glm::vec2(50, 50);
const glm::vec3 UI_ITEM_COLOR = glm::vec3(1);
const glm::vec3 UI_SELECTED_ITEM_COLOR = glm::vec3(1, 0.5, 0.5);

std::map<PowerUpType, std::string> POWER_UP_SPRITES = {
		{PowerUpType::NONE, "sword_powerup"},
		{PowerUpType::SWORD, "sword_powerup"},
		{PowerUpType::SPEAR, "spear_powerup"}};

PlayerController::PlayerController(std::shared_ptr<Player> player, GLFWwindow *window, SpriteRenderer *renderer, TextRenderer *text) : Character(player),
																																																																			 Renderer(renderer), Text(text)
{
	glfwSetKeyCallback(window, keyCallback);
}

PlayerController::~PlayerController()
{
}

void PlayerController::ProcessInput(float deltaTime)
{
	bool hasMoved = false;
	glm::vec3 direction(0.0f);

	if (keys[GLFW_KEY_A])
	{
		direction += glm::vec3(-1.0f, 0.0f, 0.0f);
		hasMoved = true;
	}
	if (keys[GLFW_KEY_D])
	{
		direction += glm::vec3(1.0f, 0.0f, 0.0f);
		hasMoved = true;
	}
	if (keys[GLFW_KEY_W])
	{
		direction += glm::vec3(0.0f, -1.0f, 0.0f);
		hasMoved = true;
	}
	if (keys[GLFW_KEY_S])
	{
		direction += glm::vec3(0.0f, 1.0f, 0.0f);
		hasMoved = true;
	}

	if (hasMoved)
	{
		Character->Move(deltaTime, direction);
	}
	else
	{
		Character->Idle();
	}

	if (keys[GLFW_KEY_SPACE])
	{
		Character->Attack();
	}

	if (keys[GLFW_KEY_1])
	{
		Character->SelectPowerUp(0);
	}

	if (keys[GLFW_KEY_2])
	{
		Character->SelectPowerUp(1);
	}
}

void PlayerController::DrawUI(glm::vec3 cameraPosition)
{
	std::vector<PowerUpType> inventory = Character->GetPowerUps();
	PowerUpType activePowerUp = Character->GetActivePowerUp();

	unsigned int space = UI_PADDING;

	for (PowerUpType powerUp : inventory)
	{
		auto color = powerUp == activePowerUp ? UI_SELECTED_ITEM_COLOR : UI_ITEM_COLOR;
		auto textureName = POWER_UP_SPRITES[powerUp];
		auto texturePosition = glm::vec3(
				cameraPosition.x + space,
				cameraPosition.y + UI_PADDING,
				0);

		Texture2D powerUpTexture = ResourceManager::GetInstance()->GetTexture(textureName);
		Renderer->DrawTexture(powerUpTexture, texturePosition, UI_ITEM_SIZE, 0, color);

		space += UI_PADDING + UI_ITEM_SIZE.x;
	}
}

void PlayerController::DrawPlayerDeath()
{
	float screenHeight = Config::Get()->GetValue(SRC_HEIGHT);

	Text->RenderText("YOU ARE DEAD", 50, screenHeight / 2, 5.0, glm::vec3(0.7, 0.0, 0.0));
}

void PlayerController::DrawObjectives()
{
	float screenWidth = Config::Get()->GetValue(SRC_WIDTH);
	float screenHeight = Config::Get()->GetValue(SRC_HEIGHT);

	Text->RenderText("Kill the Mobs", screenWidth * 3 / 4, screenHeight * 1 / 10, 1.0, glm::vec3(1.0, 1.0, 1.0));
}

glm::vec4 PlayerController::GetAttackHitbox()
{
	return Character->GetAttackHitbox();
}

glm::vec3 PlayerController::GetPosition()
{
	return Character->GetPosition();
}

bool PlayerController::IsActive()
{
	return Character->IsActive();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
			keysProcessed[key] = false;
		}
	}
}
