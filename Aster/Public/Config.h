/*
* Config singleton for configuration file
*/
#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>

using namespace std;

class Config
{
public:

	static const string FORWARD_VELOCITY;
	static const string ANGULAR_VELOCITY;
	static const string THRUST;
	static const string MASS;
	static const string FREQUENCY;
	static const string FREQUENCY_INCREASE;
	static const string BULLET_VELOCITY;
	static const string BULLET_FREQUENCY;
	static const string EXPLOSION_DURATION;
	static const string RAPID_FIRE;
	static const string DIFFICULTY_INCREASE;

	// Singletons should not be cloneable
	Config(Config& other) = delete;

	// Singletons should not be assignable
	void operator=(const Config&) = delete;

	/*
	* Static method that controls access to the singleton instance.
	* On the first run, it creates a singleton objetct and places it 
	* into the static field. On subsequent runs it returns the existing
	* object stored in the static field.
	*/
	static Config* Get();

	/*
	* Business Logic
	*/

	// Loads config from file
	void Load(const char* file);

	// Get the key selector value
	float GetValue(string key) const;

private:

	// private constructor because singletons should not create objects from outside
	Config();
	~Config();

	void StoreKeyValue(string key, string value);

	static Config* m_instance;

	map<string, float> m_config;
};

#endif