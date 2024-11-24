#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h" // Image Loader

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"


#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

float vertices[] = {
    // Positions                // Colors
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,  
    0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f, 
};

float textureCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
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


    // Using the Shader we created! Handles all the compiling, linking, etc.
    Shader shaderProgram = Shader("shader_lesson/shader.vs", "shader_lesson/shader.fs");

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex Attribute for Positions & Colors respectively
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1);
    
    /**
     * glTexParameteri
     * 1. Texture Target, either 2D or 3D
     * 2. Texture Axis! s = x, t = y, (for 3D) r = z. If its not this, it's some other texture parameter you are specifying.
     * 3. Which wrapping mode. There are 4: Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border
     * 
     * Clamp to edge - Whatever color the edge is, it extends infinitely
     * Clamp to border - any outside pixel will be some border color set. Note you'll add a border color argument to the end which will be a float[].
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    /**
     * -- Texture Filtering --
     * What happens when the pixel you are getting a coordinate for is on the border of a pixel and some other colored pixels? 
     * Nearest - picks the closest value and uses its value. This is what you used for 8 bit art in Godot. Emphasizes colors at small resolutions
     * Linear - interpolates between colors when its not centered on a pixel color
     * Min/Mag - Which options above you'd like to choose when the texture is upscaled or downscaled
     */
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    /**
     * -- Mipmaps -- 
     * When rendering faraway objects, they are a small fragment. So it gets difficult for OpenGL to pick a tex for the color of the fragment and uses a bunch of
     * wasted memory on unused textures. So mipmaps render a smaller resolution texture based on LOD. Do not use for mag filter, doesn't make sense with it. 
     * Nearest - picks closest mipmap level to render
     * Linear - interpolates between mipmap levels
     * Make sure to create the mipmap texture after loading the texture with `glGenerateMipmap`
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // Linear Mipmap, nearest texture!

    // Loading image with stb_images
    int width, height, nrChannels; // nrChannels = # of color channels
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    
    // Texture Object 
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    /**
     * glTexImage2D
     * 1. Texture Target. If GL_TEXTURE_2D/3D/1D, bound texture. Look above!
     * 2. Mipmap level, useful for manually creating mipmaps.
     * 3. Format of image to store in
     * 4 & 5. Dimensions
     * 6. Always 0. Legacy stuff
     * 7 & 8. Format & Datatype of original source image. Char = Byte
     * 9. Actual image data
     */
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data); // After creating texture & mipmaps, free image memory


    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /** Changing the Triangle Color over time to green then to black
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int ourColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        */
        // Must use glUseProgram below before we update any uniform. glUniform4f will be updating the current shader program.
        //glUniform4f(ourColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        shaderProgram.use();
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3); 
        
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