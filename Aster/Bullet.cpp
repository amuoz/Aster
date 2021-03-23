#include "Bullet.h"

#include "Common.h"
#include "Mesh.h"

#define VERTICAL_BOUND 9.0f

Bullet::Bullet(const glm::vec3 &pos, float scale, const glm::vec3 &vel)
{
	Init();

	m_position = pos;
	m_scale = glm::vec3(scale);
	m_radius = m_scale.x / 2.0f;
	m_velocity = vel;
	m_active = true;
	m_color = glm::vec3(1.0f);

	m_physicsActor = g_PhysicsPtr->AddDynamicActor(m_position, m_velocity, m_radius);
	m_physicsActor->active = true;
	m_physicsActor->report = this;
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Vertex vertex;
	vertex.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertices.push_back(vertex);

	vertex.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertices.push_back(vertex);
	vertex.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertices.push_back(vertex);

	m_mesh = new Mesh(vertices, indices, textures);
}

void Bullet::Render(Shader shader)
{
	m_position = m_physicsActor->pos;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_position);
	model = glm::scale(model, m_scale);
	//model = glm::rotate(model, (float)glfwGetTime() * m_rotAngle, m_rotAxis);

	shader.SetVector4f("color", glm::vec4(m_color, 1.0f));
	shader.SetMatrix4("model", model);

	m_mesh->Draw(shader);
}

void Bullet::Update(float deltaTime)
{
	if (m_position.y > VERTICAL_BOUND)
	{
		m_active = false;
	}
}

void Bullet::OnContact(Physics::PhysicActor* other)
{
	m_active = false;
}
