#include "Ship.h"

#include "Common.h"
#include "Config.h"
#include "Mesh.h"
#include "Game.h"
#include "Bullet.h"

#define HORIZONTAL_BOUND 12.0f

Ship::Ship(const glm::vec3 &pos, const glm::vec3 &scale)
{
	Init();

	m_alive = true;

	m_position = pos;
	m_scale = scale;
	m_radius = m_scale.x / 2.0f;
	m_velocity = glm::vec3(0.0f);
	m_thrust = Config::Get()->GetValue(Config::THRUST);
	m_mass = Config::Get()->GetValue(Config::MASS);
	m_color = glm::vec3(1.0f);

	m_physicsActor = g_PhysicsPtr->AddDynamicActor(m_position, m_velocity, m_scale, glm::vec3(0.0f), m_mass);
	m_physicsActor->active = true;
	m_physicsActor->report = this;
}

Ship::~Ship()
{

}

void Ship::Init()
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Vertex vertex;
	vertex.Position = glm::vec3(-0.5f, -0.5f, 0.2f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.1f, -0.5f, 0.2f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.0f, 0.5f, 0.0f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(-0.1f, -0.5f, 0.2f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.0f, -0.4f, 0.0f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.0f, 0.5f, 0.0f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.0f, -0.4f, 0.0f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.1f, -0.5f, 0.2f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.0f, 0.5f, 0.0f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.1f, -0.5f, 0.2f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, 0.2f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.0f, 0.5f, 0.0f);
	vertices.push_back(vertex);

	m_mesh = new Mesh(vertices, indices, textures);
}

void Ship::Render(Shader shader)
{
	m_position = m_physicsActor->pos;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_position);
	model = glm::scale(model, m_scale);
	shader.SetMatrix4("model", model);

	shader.SetVector4f("color", glm::vec4(m_color, 1.0f));
	shader.SetFloat("time", 0.0f);

	m_mesh->Draw(shader);
}

void Ship::Update(float, glm::vec4)
{
	//m_physicsActor->accelerationForce = glm::vec3(0.0f);

	if (m_position.x < -HORIZONTAL_BOUND)
	{
		m_position.x = HORIZONTAL_BOUND;
		m_physicsActor->pos = m_position;
	}
	else if (m_position.x > HORIZONTAL_BOUND)
	{
		m_position.x = -HORIZONTAL_BOUND;
		m_physicsActor->pos = m_position;
	}

	if (!m_alive)
	{
		g_game->SetGameState(GameState::GAME_RESTART);
	}
}

void Ship::OnContact(Physics::PhysicActor*)
{
	m_alive = false;
}

void Ship::Reset()
{
	m_position = glm::vec3(0.0f, -6.0f, 0.0f);
	m_physicsActor->pos = m_position;
	m_thrust = Config::Get()->GetValue(Config::THRUST);
	m_mass = Config::Get()->GetValue(Config::MASS);
	m_physicsActor->mass = m_mass;
	m_physicsActor->vel = glm::vec3(0.0f);
	m_alive = true;
}

void Ship::SetForceDirection(float direction)
{
	GetPhysicsActor()->accelerationForce = glm::vec3(direction*(m_thrust), 0.0f, 0.0f);

}

Bullet* Ship::Fire()
{
	glm::vec3 bulletPosition = glm::vec3(GetPosition()) + glm::vec3(0.0f, 0.8f, 0.0f);
	Bullet* bullet = new Bullet(bulletPosition, 0.2f, glm::vec3(0.0f, Config::Get()->GetValue(Config::BULLET_VELOCITY), 0.0f));
	bullet->SetDelete(true);	// delete from scene on restart
	return bullet;
}
