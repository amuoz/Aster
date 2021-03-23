#ifndef ASTEROID_MGR_H
#define ASTEROID_MGR_H

#include <string>
#include <iostream>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AsteroidPool.h"
#include "Actor.h"

// forwards
class Asteroid;
class Shader;

class AsteroidMgr: public Actor
{
public:
	AsteroidMgr();
	~AsteroidMgr();

	void Update(float deltaTime) override;
	void Render(Shader shader) override;

	void Reset();

private:
	// active asteroids
	std::list<Asteroid*> m_asteroids;	

	// Asteroid pool
	AsteroidPool<Asteroid>* m_pool;

	// difficulty colors
	glm::vec3 m_colors[4];

	// time accumulation to spawn
	float m_timeAccum;

	// current spawn frequency
	float m_currentFreq;

	// difficulty variables
	int m_difficultyIndex;
	int m_lastDifficultyIndex;
	float m_currentForwardVelocity;

	void SpawnAsteroid();

};

#endif
