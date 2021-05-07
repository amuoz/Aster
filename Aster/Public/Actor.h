#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Physics.h"
#include "Mesh.h"

class SpriteRenderer;
class Sprite;

class Actor
{
public:

	Actor();
	Actor(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Actor();

	virtual void Render(Shader shader) = 0;
	virtual void Update(float deltaTime, glm::vec4 playerAttackHitbox) = 0;
	virtual void Reset();
	virtual void Draw(SpriteRenderer &renderer, double deltatime);

	void SetActive(bool newActive);
	void SetDelete(bool newDelete);
	void SetColor(glm::vec3 color);

	inline bool IsActive() { return m_active; }
	inline bool IsDelete() { return m_delete; }
	inline Physics::PhysicActor* GetPhysicsActor() { return m_physicsActor; }
	inline Mesh* GetMesh() { return m_mesh; }
	
	inline glm::vec3 GetPosition() { return m_position; }
	void SetPosition(glm::vec3 pos);
	bool IsAttacked(glm::vec4 attackHitbox);

	bool IsDestroyable;
	bool IsDestroyed;

protected:
	// actor state
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_velocity;
	float m_rotAngle;
	glm::vec3 m_rotAxis;
	float m_radius;
	glm::vec3 m_color;
	

	bool m_active;

	// Actor should be deleted on restart
	bool m_delete;

	// physics pointer
	Physics::PhysicActor* m_physicsActor = nullptr;

	// render state
	Mesh* m_mesh = nullptr;
	Sprite* m_sprite;
};
