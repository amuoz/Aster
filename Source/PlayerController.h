#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <memory>
#include <map>

#include "Common.h"
#include "AsterTypes.h"

class Player;
struct GLFWwindow;
class SpriteRenderer;
class TextRenderer;

class PlayerController
{
public:
	PlayerController(GLFWwindow *window, SpriteRenderer *renderer, TextRenderer *text);
	~PlayerController();

	void ProcessInput(float deltaTime);
	void DrawUI(glm::vec3 cameraPosition);
	void DrawPlayerDeath();
	void DrawObjectives();
	glm::vec4 GetAttackHitbox();
	glm::vec2 GetPosition();
	bool IsActive();

	// Player posession on runtime
	void Posses(std::shared_ptr<Player> player);

private:
	const glm::vec3 UI_ITEM_COLOR = glm::vec3(1);
	const glm::vec3 UI_SELECTED_ITEM_COLOR = glm::vec3(1, 0.5, 0.5);

	std::map<PowerUpType, std::string> POWER_UP_SPRITES = {
			{PowerUpType::SWORD, "sword_powerup"},
			{PowerUpType::SPEAR, "spear_powerup"},
			{PowerUpType::HAMMER, "hammer_powerup"}};

	GLFWwindow *Window;

	std::shared_ptr<Player> Character;
	SpriteRenderer *Renderer;
	TextRenderer *Text;
};
