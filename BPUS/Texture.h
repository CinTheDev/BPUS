#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Image.h"

#include "shaderClass.h"

class Texture {
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	Texture(int r, int g, int b, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	// Assign texture unit to texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};

#endif