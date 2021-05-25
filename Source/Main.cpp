#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Config.h"
#include "ResourceManager.h"
#include <iostream>

#include "Game.h"
//std::unique_ptr<Game> g_game;

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int, char**)
{
	// GLFW: initialize and configure
	std::cout << "...INIT GLFW CONTEXT..." << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	// Load file config properties
	Config::Get()->Load(CONFIG_FILE);
	
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(
		(int)Config::Get()->GetValue(SRC_WIDTH),
		(int)Config::Get()->GetValue(SRC_HEIGHT),
		"Aster", 0, 0);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// GLFW to capture our mouse

	// Glad: load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Glad GLAD is working
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	// Configure global OPENGL state
	glViewport(
		0,
		0,
		GLsizei(Config::Get()->GetValue(SRC_WIDTH)),
		GLsizei(Config::Get()->GetValue(SRC_HEIGHT))
	);

	//glEnable(GL_DEPTH_TEST);	// depth testing not needed in 2D game
	glEnable(GL_BLEND);			// for alpha render
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << "...INIT GAME..." << std::endl;

	// Load dependencies and construct all objects
	Game::Get()->InitGame(window);

	// BeginPlay logic with all objects ready
	Game::Get()->BeginPlay();

	// deltaTime variables
	float m_deltaTime = 0.0f;	// Time between current frame and last frame
	float m_lastFrame = 0.0f;	// Time of last frame
	float TimeStepAccum = 0.0f;
	float fixedUpdateFreq = 1.0f / 60.0f;	// fixed time step

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		TimeStepAccum += m_deltaTime;
		while (TimeStepAccum > fixedUpdateFreq)
		{
			TimeStepAccum -= fixedUpdateFreq;

			Game::Get()->Execute(fixedUpdateFreq);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	// delete all resources as loaded using the resource manager
	ResourceManager::GetInstance()->Clear();

	glfwTerminate();
	return 0;
}

// GLFW: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
