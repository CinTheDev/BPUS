#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "../renderer/shaderclass/shaderclass.h"
#include "../renderer/VAO/VAO.h"
#include "../renderer/VBO/VBO.h"
#include "../renderer/EBO/EBO.h"

#include "../utils/utils.cpp"
#include "../renderer/renderer.cpp"
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
        // In case the window fails
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
    shader shaderProgram("renderer/shader/default.vert", "renderer/shader/default.frag");
    shaderProgram.Activate();

    // Get ID of uniform
    //GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    float deltatime = 0.16;
    auto frameBegin = std::chrono::system_clock::now();

    BPUS_game* game = new BPUS_game(window, shaderProgram);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        updateArguments upArgs = updateArguments();
        upArgs.window = window;
        upArgs.deltatime = deltatime;

        game->update(upArgs);
        obj_m::objects_update(upArgs);

        renderArguments reArgs = renderArguments();
        reArgs.window = window;
        reArgs.shaderProgram = &shaderProgram;

        render(reArgs);

        auto frameEnd = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameBegin);
        deltatime = elapsed.count() / 1000.0;

        frameBegin = frameEnd;
    }

    // Delete and terminate everything
    delete game;
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
