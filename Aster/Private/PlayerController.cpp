#include "PlayerController.h"

#include <utility>
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Player.h"
#include "Config.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

bool keys[1024];
bool keysProcessed[1024];

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

PlayerController::PlayerController(std::shared_ptr<Player> player, GLFWwindow *window, SpriteRenderer *renderer, TextRenderer *text)
		: Character(player), Renderer(renderer), Text(text)
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
}

void PlayerController::DrawUI()
{
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
