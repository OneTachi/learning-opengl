#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

// Vertices we'll use in the form of (x,y,z). Note that z is our depth (front/back of object)
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
    0.0f, 0.5f, 0.0f
};

int main() 
{
    glfwInit();
    // Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /**
     * Our Vertex Buffer Object (VBO) => a place we store large amounts of vertex memory in GPU for the vertex shader
     * We send this all at once cause sending info from CPU -> GPU is slow! (Thank goodness for CISC360)
     * This is why we have a bunch of memory on GPU it seems.
     */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Here we actually bind our object to the VBO Buffer. We could bind our object to multiple buffers! 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /**   
     * Copy in our vertice data to the buffer. The last argument tells OpenGL how frequent our buffer data changes and is used by the GPU.
     * 
     * --GL_X_Draw--
     * Static: Doesn't Change + High Frequency
     * Stream: Doesn't Change + Used a couple times
     * Dynamic: Changes a lot + High Frequency
     * 
     * We don't plan on changing VBO and it will be used a lot. => GL_STATIC_DRAW
    */ 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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