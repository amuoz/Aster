#include "Player.h"

Player::Player(): Actor()
{

}

Player::Player(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity): Actor(pos, size, sprite, color, velocity)
{

}

Player::~Player()
{

}

void Player::Render(Shader)
{

}

void Player::Update(float)
{

}

void Player::Move(float deltaTime, glm::vec3 direction)
{
	float velocity = PLAYER_VELOCITY * deltaTime;
	m_position += velocity * direction;
}
