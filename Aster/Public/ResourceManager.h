#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture2D.h"
#include "Shader.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:

	// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
	// retrieves a stored sader
	Shader GetShader(std::string name);
	// loads (and generates) a texture from file
	Texture2D LoadTexture(const char *file, bool alpha, std::string name);
	// retrieves a stored texture
	Texture2D GetTexture(std::string name);
	// properly de-allocates all loaded resources
	void Clear();

	static ResourceManager* GetInstance();

private:
	// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager();
	~ResourceManager();

	static ResourceManager* m_instance;

	// loads and generates a shader from file
	Shader LoadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
	// loads a single texture from file
	Texture2D LoadTextureFromFile(const char *file, bool alpha);

	// resource storage
	std::map<std::string, Shader> Shaders;
	std::map<std::string, Texture2D> Textures;
};

#endif