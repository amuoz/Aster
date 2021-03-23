#include "Common.h"
#include "AsteroidPool.h"
//#include "Asteroid.h"

/*
template <class T>
AsteroidPool<T>::AsteroidPool()
{

}

template <class T>
AsteroidPool<T>::~AsteroidPool() {
	while (m_pool.size())
	{
		T* obj = m_pool.front();
		m_pool.pop_front();
		delete obj;
	}
}

template <class T>
T* AsteroidPool<T>::getAsteroid()
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

template <class T>
void AsteroidPool<T>::returnAsteroid(T* resource)
{
	resource->Reset();
	m_pool.push_back(resource);
}
*/
