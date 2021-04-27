#ifndef ASTEROID_H
#define ASTEROID_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Actor.h"
#include "IPoolable.h"
#include "Physics.h"
#include "Config.h"
#include "Game.h"

#include "ICircleContactReport.h"

class Mesh;
class Shader;

class Asteroid: public Actor, IPoolable, ICircleContactReport
{
public:

	Asteroid();
	~Asteroid();

	void Init();

	// IPoolable
	void Reset() override;

	void Render(Shader shader) override;
	void Update(float deltaTime, glm::vec4) override;

	float Randf(float min, float max);

	bool OutOfBounds();

	void OnContact(Physics::PhysicActor* other) override;

	bool HasExploded();

private:

	bool m_exploded;
	bool m_explosion;
	float m_explosionTime;
	float m_explosionDuration;
	float m_lastRotAngle;
};

#endif