#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../utils/image/image.h"

#include "../../renderer/shaderclass/shaderclass.h"

class texture {
public:
    GLuint ID;
    GLenum type;
    texture(const char* imageFile, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    // Assign texture unit to texture
    void texUnit(shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif
