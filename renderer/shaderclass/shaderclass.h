#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class shader {
public:
    // Reference ID
    GLuint ID;
    // Constructor with 2 shaders
    shader(const char* vertexFile, const char* fragmentFile);

    // Activate shader program
    void Activate();
    // Delete shader program
    void Delete();

private:
    void compileErrors(unsigned int shader, const char* type);
};

#endif
