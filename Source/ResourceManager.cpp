#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>

#include "Config.h"
#include "Level.h"
#include "Common.h"
#include "BuildingManager.h"
#include "TileBuilder.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	if (m_instance)
	{
		delete m_instance;
	}
}

Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
	Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
	Textures[name] = LoadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

std::shared_ptr<Level> ResourceManager::LoadLevel(const char *file, std::string name)
{
	Levels[name] = LoadLevelFromFile(file);
	return Levels[name];
}

std::shared_ptr<Level> ResourceManager::GetLevel(std::string name)
{
	return Levels[name];
}

void ResourceManager::Clear()
{
	// (properly) delete all shaders
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

ResourceManager *ResourceManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager();
	}
	return m_instance;
}

Shader ResourceManager::LoadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	const char *gShaderCode = geometryCode.c_str();
	// 2. now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char *file, bool alpha)
{
	// create texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// load image
	int width, height, nrChannels;
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	// now generate texture
	texture.Generate(width, height, data);
	// and finally free image data
	stbi_image_free(data);
	return texture;
}

std::shared_ptr<Level> ResourceManager::LoadLevelFromFile(const char *file)
{
	auto tileBuilder = std::make_shared<TileBuilder>();
	auto buildingManager = std::make_shared<BuildingManager>(tileBuilder);
	auto level = std::make_shared<Level>(buildingManager, tileBuilder);
	level->Load(file);

	return level;
}