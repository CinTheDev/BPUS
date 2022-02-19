#include "shaderclass.h"
#include <vector>

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

// Constructor that builds the Shader Program from 2 different shaders
shader::shader(const char* vertexFile, const char* fragmentFile) {
    // Read vertexFile and fragmentFile as strings
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    // Convert the shader source strings into char arrays
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Create Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach the Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile the Fragment Shader into machine code
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    // Create Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Sahder Program
    glLinkProgram(ID);
    compileErrors(ID, "PROGRAM");

    // Delete the now useless Vertex and Fragment Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activates the shader program
void shader::Activate() {
    glUseProgram(ID);
}

// Deletes the Shader Program
void shader::Delete() {
    glDeleteProgram(ID);
}

void shader::compileErrors(GLuint shader, const char* type) {
    GLint hasCompiled;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, NULL, &infoLog[0]);
            std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            glGetProgramInfoLog(shader, 1024, &maxLength, &infoLog[0]);
            std::cout << "SHADER_LINKING_ERROR for: " << type << std::endl;
            std::cout << infoLog << std::endl;
        }
    }
}
