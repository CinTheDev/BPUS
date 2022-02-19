#include "texture.h"

texture::texture(const char* imageFile, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    // Assign the type of texture to the texture object
    type = texType;

    // Read the image
    image* img = new image(imageFile);

    // Generate an OpenGL texture object
    glGenTextures(1, &ID);
    // Assign the texture to a Texture Unit
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    // Configure the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configure the way the texture repeats (if it does at all)
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Assign the image to the OpenGL Texture object
    glTexImage2D(texType, 0, GL_RGBA, img->w, img->h, 0, format, pixelType, img->data);
    // Generate Mipmaps
    glGenerateMipmap(texType);

    // Delete Image Object as the data is in the Texture Object now
    delete img;

    // Unbind the OpenGL Texture object so that it cannot accidentally be modified
    glBindTexture(texType, 0);
}

void texture::texUnit(shader& shader, const char* uniform, GLuint unit) {
    // Get the location of the uniform
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    // Shader needs to be activated before changing the value of a uniform
    shader.Activate();
    // Set the value of the uniform
    glUniform1i(texUni, unit);
}

void texture::Bind() {
    glBindTexture(type, ID);
}

void texture::Unbind() {
    glBindTexture(type, 0);
}

void texture::Delete() {
    glDeleteTextures(1, &ID);
}
