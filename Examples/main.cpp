#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "engine.h"
#include <iostream>

Engine g_Engine("Animation", 1200, 800);

void errorCallback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        g_Engine.KeyPress(key);
    }
    else if (action == GLFW_RELEASE) {
        g_Engine.KeyRelease(key);
    }
}

void renderCallback() {
    g_Engine.Render();
}

void updateCallback() {
    g_Engine.Update();
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    g_Engine.Resize(width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    g_Engine.MouseMotion(xpos, ypos);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    const int WindowWidth = 1200;
    const int WindowHeight = 800;

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "GLFW Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set callback functions
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowSizeCallback(window, windowResizeCallback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewInit();

    g_Engine.Initialize();
    g_Engine.LoadWorld("Demo1.txt");

    while (!glfwWindowShouldClose(window)) {
        // Update
        updateCallback();

        // Render
        renderCallback();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
