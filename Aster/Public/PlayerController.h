#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <memory>

class Player;
class GLFWwindow;
class SpriteRenderer;
class TextRenderer;

class PlayerController
{
public:
	PlayerController(std::shared_ptr<Player> player, GLFWwindow *window, SpriteRenderer *renderer, TextRenderer *text);
	~PlayerController();

	void ProcessInput(float deltaTime);
	void DrawUI(glm::vec3 position);
	void DrawPlayerDeath();
	void DrawObjectives();
	glm::vec4 GetAttackHitbox();
	glm::vec3 GetPosition();
	bool IsActive();

private:
	std::shared_ptr<Player> Character;
	SpriteRenderer *Renderer;
	TextRenderer *Text;
};
