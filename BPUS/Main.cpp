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
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Bad Piggies Ultimate Sandbox", NULL, NULL);
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

	// Generate default shader
	Shader shaderProgram("default.vert", "default.frag");

	// Get ID of uniform
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	 // Example texture
	Texture popCat("Assets/Images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	Obj::Baseobject bso = Obj::Baseobject(Vector2(-0.65, -0.5), &popCat, Vector2(0.6, 1));
	Obj_M::create(&bso);

	Input input = {};

	float deltatime = 0.16f;
	LARGE_INTEGER frameBeginTime;
	QueryPerformanceCounter(&frameBeginTime);

	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	BPUS_Game* game = new BPUS_Game();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		game->update(&input, deltatime);

		RenderArguments args = RenderArguments();
		args.window = window;
		args.shader = &shaderProgram;
		args.scaleUni = uniID;

		args.testObject = &bso;

		render(args);

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		deltatime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
		frameBeginTime = frameEndTime;
	}
	
	// Delete and terminate everything
	popCat.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
