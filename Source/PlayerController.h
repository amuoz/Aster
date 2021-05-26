#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <memory>

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

	GLFWwindow* Window;
	
	std::shared_ptr<Player> Character;
	SpriteRenderer *Renderer;
	TextRenderer *Text;


};
