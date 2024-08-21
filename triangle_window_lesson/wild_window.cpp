#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

int main() 
{
    glfwInit();
    // Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set Profile to Core (Gives us more control over shader programs)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window object! We can have multiple of these for our program it seems.
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    // Is our window there?
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Does Glad work?
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    /** 
     * Create our viewport, it's our actual window (OpenGL function). 0,0 is the value of the most bottom left pixel (position wise). 
     * The other two arguments are for our screen size. 
     * If it's bigger than GLFW window, we render to a small box inside of the viewport. Smaller? Stuff gets cropped out I think 
     * */
    glViewport(0, 0, 800, 600);

    // Callback function for when we resize the window!
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Simple Render Loop
    while (!glfwWindowShouldClose(window)) 
    {
        // Input
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Always call this last to make sure there are no leftover artifacts from glfw. 
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
        glViewport(0, 0, 800, 600);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}