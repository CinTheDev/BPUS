#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "utils.cpp"
#include "renderer.cpp"
#include "game.cpp"

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	
	// Create window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "BPUS", NULL, NULL);
	if (window == NULL) {
		// If the window fails to create
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	// Specify viewport of OpenGL
	glViewport(0, 0, 1280, 720);

	// Enable Alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_MULTISAMPLE);

	// Generate default shader
	Shader shaderProgram("default.vert", "default.frag");

	// Get ID of uniform
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	float deltatime = 0.16f;
	LARGE_INTEGER frameBeginTime;
	QueryPerformanceCounter(&frameBeginTime);

	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	BPUS_Game* game = new BPUS_Game(window, shaderProgram);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		UpdateArguments upArgs = UpdateArguments();
		upArgs.window = window;
		upArgs.deltaTime = deltatime;

		game->update(upArgs);

		RenderArguments reArgs = RenderArguments();
		reArgs.window = window;
		reArgs.shader = &shaderProgram;
		reArgs.scaleUni = uniID;

		render(reArgs);

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		deltatime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
		frameBeginTime = frameEndTime;
	}
	
	// Delete and terminate everything
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
