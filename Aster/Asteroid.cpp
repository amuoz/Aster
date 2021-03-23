#include "Asteroid.h"
#include "Common.h"
#include "Mesh.h"

#include <GLFW/glfw3.h>

#define PI 3.1415926535897932

#define VERTICAL_BOUND 15.0f
#define HORIZONTAL_BOUND 20.0f

Asteroid::Asteroid()
{
	Init();

	Reset();

	m_physicsActor = g_PhysicsPtr->AddDynamicActor(m_position, m_velocity, m_radius);
	m_physicsActor->report = this;
}

Asteroid::~Asteroid()
{
}

void Asteroid::Init()
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Vertex vertex;

	vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(vertex);

	m_mesh = new Mesh(vertices, indices, textures);
}

void Asteroid::Render(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	if (!m_explosion) 
	{
		m_position = m_physicsActor->pos;

		model = glm::translate(model, m_position);
		model = glm::scale(model, m_scale);
		m_lastRotAngle = (float)glfwGetTime() * m_rotAngle;
		model = glm::rotate(model, m_lastRotAngle, m_rotAxis);
	}
	else 
	{
		model = glm::translate(model, m_position);
		model = glm::scale(model, m_scale);
		model = glm::rotate(model, m_lastRotAngle, m_rotAxis);
	}

	shader.SetMatrix4("model", model);

	shader.SetVector4f("color", glm::vec4(m_color, 1-m_explosionTime));
	shader.SetFloat("time", m_explosionTime);

	m_mesh->Draw(shader);
}

void Asteroid::Update(float deltaTime)
{
	if (m_explosion)
	{
		m_explosionTime += deltaTime;

		if (m_explosionTime >= Config::GetInstance()->GetValue(Config::EXPLOSION_DURATION))
		{
			m_exploded = true;
		}
	}
}

float Asteroid::Randf(float min, float max)
{
	return (float)(((rand() & 32767)*(1.0 / 32767.0))*(max - min) + min);
}

bool Asteroid::OutOfBounds()
{
	if (m_position.y < -VERTICAL_BOUND 
		|| m_position.y > VERTICAL_BOUND*2 
		|| m_position.x < -HORIZONTAL_BOUND 
		|| m_position.x > HORIZONTAL_BOUND)
	{
		return true;
	}
	return false;
}

void Asteroid::OnContact(Physics::PhysicActor* other)
{
	if (!other->ignoreContact)
	{
		m_explosion = true;
		m_physicsActor->active = false;
		// score++
		g_game->IncreaseScore();
	}
}

bool Asteroid::HasExploded()
{
	return m_exploded;
}

void Asteroid::Reset()
{	
	float offset = 8.0f;
	// 1. translation: displace along circle with 'radius' in range [-offset, offset]
	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	float x = displacement;
	float y = (rand() % 10) + 10.0f; // keep height of field smaller compared to width of x and z

	// 2. scale: scale between 0.05 and 0.25f / 0.5 and 1
	float scale = (rand() % 100) / 100.0f + 0.5;

	// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
	float rotAngle = Randf(Config::GetInstance()->GetValue(Config::ANGULAR_VELOCITY) / 2, 
		Config::GetInstance()->GetValue(Config::ANGULAR_VELOCITY));

	m_position = glm::vec3(x, y, 0.0f);
	m_scale = glm::vec3(scale);
	m_rotAngle = rotAngle;
	m_rotAxis = glm::vec3((float)rand(), (float)rand(), (float)rand());
	m_radius = m_scale.x / 1.6f;
	m_velocity = glm::vec3(0.0f, -(Config::GetInstance()->GetValue(Config::FORWARD_VELOCITY)), 0.0f);
	m_exploded = false;
	m_explosion = false;
	m_explosionTime = 0.0f;
	m_color	= glm::vec3(40.0f/255, 180.0f/255, 100.0f/255);

	if (m_physicsActor)
	{
		SetActive(false);
		m_physicsActor->pos = m_position;
		m_physicsActor->report = this;
		m_physicsActor->ignoreContact = true;
	}
}
