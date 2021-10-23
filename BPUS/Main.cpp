#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Image.h"

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.1f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.1f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

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

	// Generate Vertex Array Object and bind
	VAO VAO1;
	VAO1.Bind();

	// Generate Vertex Buffer Object and link to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer Object and link to indices
	EBO EBO1(indices, sizeof(indices));

	// Link VBO attributes to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidental modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Get ID of uniform
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	 // Example texture
	Texture popCat("Assets/Images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Specify color of background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Use default shader
		shaderProgram.Activate();
		// Assign value to uniform
		glUniform1f(uniID, 0.5f);
		// Bind texture
		popCat.Bind();
		// Bind VAO
		VAO1.Bind();
		// Draw primitives
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap front and back buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	
	// Delete and terminate everything
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
