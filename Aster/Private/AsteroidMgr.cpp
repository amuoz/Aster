#include "AsteroidMgr.h"
#include "Asteroid.h"
#include "Common.h"

#include <algorithm>

#define POOL_SIZE 5
#define MAX_FREQ 0.1f

AsteroidMgr::AsteroidMgr()
{
	m_difficultyIndex = 0;
	m_timeAccum = 0.0f;
	m_currentFreq = Config::Get()->GetValue(Config::FREQUENCY);
	m_pool = new AsteroidPool<Asteroid>();
	
	// green		40, 180, 99
	// yellow		244, 208, 63
	// orange		241, 150, 25
	// red			234, 50, 38
	m_colors[3] = glm::vec3(40.0f / 255, 180.0f / 255, 99.0f / 255);
	m_colors[2] = glm::vec3(244.0f / 255, 208.0f / 255, 63.0f / 255);
	m_colors[1] = glm::vec3(241.0f / 255, 150.0f / 255, 25.0f / 255);
	m_colors[0] = glm::vec3(234.0f / 255, 50.0f / 255, 38.0f / 255);
	
	m_lastDifficultyIndex = 3;
	m_currentForwardVelocity = Config::Get()->GetValue(Config::FORWARD_VELOCITY);
}

AsteroidMgr::~AsteroidMgr()
{
	delete m_pool;
	m_asteroids.clear();
	// deallocating the memory
	std::list<Asteroid*>().swap(m_asteroids);
}

void AsteroidMgr::Update(float deltaTime, glm::vec4 hitbox)
{
	m_timeAccum += deltaTime;
	// spawn freq increases over time
	m_currentFreq -= Config::Get()->GetValue(Config::FREQUENCY_INCREASE) * deltaTime;
	// clamp max freq
	m_currentFreq = static_cast<float>(std::max<float>(MAX_FREQ, std::min<float>(m_currentFreq, Config::Get()->GetValue(Config::FREQUENCY))));

	m_difficultyIndex = static_cast<int>(std::trunc(m_currentFreq / (Config::Get()->GetValue(Config::FREQUENCY) / 4)));
	if (m_lastDifficultyIndex != m_difficultyIndex)
	{
		m_currentForwardVelocity *= Config::Get()->GetValue(Config::DIFFICULTY_INCREASE);
	}
	m_lastDifficultyIndex = m_difficultyIndex;

	for (std::list<Asteroid*>::iterator it = m_asteroids.begin(); it != m_asteroids.end();)
	{
		Asteroid* asteroid = (*it);
		asteroid->Update(deltaTime, hitbox);
		
		// if out-of-bounds recycle asteroid
		if (asteroid->OutOfBounds() || asteroid->HasExploded())
		{
			it = m_asteroids.erase(it);
			m_pool->returnAsteroid(asteroid);
		}
		else
		{
			++it;
		}
	}

	// spawn asteriod by frequency
	if (m_timeAccum >= m_currentFreq)
	{
		m_timeAccum -= m_currentFreq;
		SpawnAsteroid();
	}

}

void AsteroidMgr::Render(Shader shader)
{
	for (std::list<Asteroid*>::iterator it = m_asteroids.begin(); it != m_asteroids.end(); ++it)
	{	
		Asteroid* asteroid = (*it);
		asteroid->Render(shader);
	}
}

void AsteroidMgr::Reset()
{
	// return all asteroids to the pool
	for (std::list<Asteroid*>::iterator it = m_asteroids.begin(); it != m_asteroids.end();)
	{
		Asteroid* asteroid = (*it);
		it = m_asteroids.erase(it);
		m_pool->returnAsteroid(asteroid);
	}

	m_timeAccum = 0.0f;
	m_currentFreq = Config::Get()->GetValue(Config::FREQUENCY);
	m_lastDifficultyIndex = 3;
	m_currentForwardVelocity = Config::Get()->GetValue(Config::FORWARD_VELOCITY);
}

void AsteroidMgr::SpawnAsteroid()
{
	Asteroid* asteroid = m_pool->getAsteroid();
	asteroid->Reset();

	// current difficulty configuration
	// --------------------------------
	asteroid->SetColor(m_colors[m_difficultyIndex]);
	float offset = 1.0f;
	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	asteroid->GetPhysicsActor()->vel = glm::vec3(displacement, -(m_currentForwardVelocity), 0.0f);
	//asteroid->m_physicsActor->ignoreContact = true;
	
	// activate actor
	asteroid->SetActive(true);
	m_asteroids.push_back(asteroid);
}

