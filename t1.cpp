#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

// Vertices we'll use in the form of (x,y,z). Note that z is our depth (front/back of object)
float vertices[] = {
    -0.5f, -0.5f, 0.0f, // BL
    0.5f, -0.5f, 0.0f, // BR
    0.5f, 0.5f, 0.0f, // TR
};

float otherVertices[] = {
    0.5f, -0.5f, 0.0f,
    0.9f, -0.5f, 0.0f,
    .9f, .5f, 0.0f,    
};

unsigned int indices[] = {
    0, 3, 2,
    1, 2, 0 
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


    // Our Vertex Shader we made. Bigger shaders probably will be linked through different files rather than doing making it here.
    const char *vertexShaderSource = "#version 330 core\n" 
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // The 1 signifies how many strings we're passing as a source code.
    glCompileShader(vertexShader); // Compile Shader

    // Check to see if compilation is successfull!
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // command to check compilation
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get info from failed compilation
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Get info from failed compilation
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // Create Shader Program by linking our shaders in!
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check Shader Program (different from shader checks)
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // No longer need shader objects as they've been linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /**
     * Our Vertex Buffer Object (VBO) => a place we store large amounts of vertex memory in GPU for the vertex shader
     * We send this all at once cause sending info from CPU -> GPU is slow! (Thank goodness for CISC360)
     * This is why we have a bunch of memory on GPU it seems.
     */
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    /**
     * Element buffer object. This is to prevent us from repeating points in our vertices when making our rectangle!
     */
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    /** 
     * VAO's stores all below vertex configuration as a state. Required to do this in Core. This is useful when we get 100's of different associated vertex attributes 
     * Remember to do glGenVertexArrays for new VAO before you call Vertex Attribution calls
     * THIS ALSO STORES ASSOCIATED VBO DATA
     */ 
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Actually Bind VAO to configure
    glBindVertexArray(VAO);


    // Here we actually bind our object to the VBO Buffer. We could bind our object to multiple buffers! Do After VAO
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


    /** 
     * All the same as we've done with our VBO! Just some replacements with the Type!
     * Make sure not to unbind the EBO before you unbind VAO. Otherwise th
    */
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /**
     * Specify how are vertex attributes are formatted to OpenGL. Also binds VBO to current VAO.
     * -- Params for glVertexAtrribPointer --
     * 1. What attribute data to configure. Remember we used layout (location = 0) in our vertex shader to specify where we are getting coords from.
     * 2. Partitions of a single vertex attribute. 3 in our case (x, y, z)
     * 3. Type of Data
     * 4. Do you want to normalize the values? (We already did it ourself so no)
     * 5. Stride -> Space between indices of attribute data. Since ours is tightly packed, we can just put the size of the attribute.
     * 6. Offset. Requires wacky cast
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // GL_ARRAY_BUFFER is associated with 0 when above function is called I think?
    
    unsigned int VBO2, VAO2; 
    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &VAO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(otherVertices), otherVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Simple Render Loop
    while (!glfwWindowShouldClose(window)) 
    {
        // Input
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
 
        glUseProgram(shaderProgram); // Use our shader program till we change it with same function
        glBindVertexArray(VAO); // Choose Specification
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw. Params: Type of Drawing, start index, end index
        glBindVertexArray(VAO2); // Choose Specification
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw. Params: Type of Drawing, start index, end index
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode, What to wireframe and how
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