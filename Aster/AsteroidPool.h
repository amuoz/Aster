#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <string>
#include <iostream>
#include <list>

#include "IPoolable.h"

template <class T = IPoolable>
class AsteroidPool
{
private:
	std::list<T*> m_pool;

public:

	AsteroidPool()
	{

	}

	~AsteroidPool() {
		while (m_pool.size())
		{
			T* obj = m_pool.front();
			m_pool.pop_front();
			delete obj;
		}
	}
	/**
	 * Returns instance of Asteroid.
	 *
	 * New resource will be created if all the resources
	 * were used at the time of the request.
	 */
	T* getAsteroid()
	{
		if (m_pool.empty())
		{
			T* resource = new T();
			//std::cout << "New Asteroid created!" << std::endl;
			return resource;
		}
		else
		{
			T* resource = m_pool.front();
			m_pool.pop_front();
			//std::cout << "Asteroid reused!" << std::endl;
			return resource;
		}
	}
	
	/**
	 * Return asteroid back to the pool.
	 *
	 * The resource must be initialized back to
	 * the default settings before someone else
	 * attempts to use it.
	 */
	void returnAsteroid(T* resource)
	{
		resource->Reset();
		m_pool.push_back(resource);
	}

};


#endif