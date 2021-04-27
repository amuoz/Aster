#ifndef SHIP_H
#define SHIP_H

#include "Actor.h"
#include "Physics.h"
#include "ICircleContactReport.h"

const float SHIP_SPEED = 5.0f;

class Shader;
class Bullet;

//Used as abstraction to stay away from window-system specific input methods
enum class Ship_Movement {
	L,
	R
};

class Ship: public Actor, ICircleContactReport
{
public:

	Ship(const glm::vec3 &pos, const glm::vec3 &scale);
	~Ship();

	void Init();

	void Render(Shader shader) override;

	void Update(float deltaTime, glm::vec4) override;

	void OnContact(Physics::PhysicActor* other) override;

	void Reset() override;

	void SetForceDirection(float direction);

	Bullet* Fire();

private:

	// accerelated horizontal movement
	float m_thrust;
	float m_mass;

	bool m_alive;
};

#endif