#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
	// Assign the type of texture to the texture object
	type = texType;

	// Read the image
	Image* img = new Image(image);

	// Generate an OpenGL texture object
	glGenTextures(1, &ID);
	// Assign the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configure the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure the way the texture repeats (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assign the image to the OpenGL Texture object
	glTexImage2D(texType, 0, GL_RGBA, img->w, img->h, 0, format, pixelType, img->data);
	// Generate MipMaps
	glGenerateMipmap(texType);

	// Delete Image Object as the data is in the Texture Object
	delete img;

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(texType, 0);
}

Texture::Texture(int r, int g, int b, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
	// Assign the type of texture to the texture object
	type = texType;

	// Read the image
	uint8_t* data = new uint8_t[3];
	data[0] = r;
	data[1] = g;
	data[2] = b;

	// Generate an OpenGL texture object
	glGenTextures(1, &ID);
	// Assign the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configure the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure the way the texture repeats (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assign the image to the OpenGL Texture object
	glTexImage2D(texType, 0, GL_RGBA, 1, 1, 0, format, pixelType, data);
	// Generate MipMaps
	glGenerateMipmap(texType);

	// Delete Image Object as the data is in the Texture Object
	delete[] data;

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	// Get the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Set the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}
