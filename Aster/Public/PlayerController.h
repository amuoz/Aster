#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <memory>

class Player;
class GLFWwindow;

class PlayerController
{
public:
	PlayerController(std::shared_ptr<Player> player, GLFWwindow *window);
	~PlayerController();

	void ProcessInput(float deltaTime);
	glm::vec4 GetAttackHitbox();
	glm::vec3 GetPosition();
	bool IsActive();

private:
	std::shared_ptr<Player> Character;
};
