#include "Config.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Config *Config::m_instance = nullptr;

Config::Config()
{
}

Config::~Config()
{
	if (m_instance)
	{
		delete m_instance;
	}
}

void Config::Load(const char *file)
{
	std::cout << "... Load " << file << " ..." << std::endl;
	m_config.clear();

	// load from file
	//unsigned int tileCode;
	std::string line;
	std::ifstream fstream(file);
	if (fstream)
	{
		// read each line from config file
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::string key;
			if (std::getline(sstream, key, '='))
			{
				std::string value;
				if (std::getline(sstream, value))
				{
					StoreKeyValue(key, value);
				}
			}
		}
	}
}

float Config::GetValue(string key) const
{
	return m_config.find(key)->second;
}

Config *Config::Get()
{
	if (m_instance == nullptr)
	{
		m_instance = new Config();
	}
	return m_instance;
}

void Config::StoreKeyValue(string key, string value)
{
	std::string::size_type sz; // alias of size_t
	float floatValue = std::stof(value, &sz);
	m_config.insert(std::pair<string, float>(key, floatValue));
}
