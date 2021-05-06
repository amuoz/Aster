#include "PlayerController.h"

#include <utility>
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Player.h"
#include "Common.h"

bool keys[1024];
bool keysProcessed[1024];

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

PlayerController::PlayerController(std::shared_ptr<Player> player, GLFWwindow *window)
		: Character(player)
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
