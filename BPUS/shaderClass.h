#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
public:
	// Reference ID
	GLuint ID;
	// Constructer with 2 shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activate Shader Program
	void Activate();
	// Delete Shader Program
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif