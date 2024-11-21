#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
public:
    // Program ID
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    // Activate shader
    void use();

    // Utility Functions for setting Uniform values
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

};

/**
 * Reads vertex/fragment shaders from its file & compiles them.
 * Lots of cool stuff I've learned!
 */
Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
    // 1. Retrieve source code from files
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // Set what exceptions to flag
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit); // Note syntax. | => OR Operation!
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit); // Also you don't need the space between func and (). But C++ docs does this.

    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        /**
         * StringStream's to grab the string data from the files.
         * Ifstreams use rdbuf() which gives access to the internal buf file. 
         * And stringstreams use the in operator (<<) to grab stuff when it has access to a buf file. 
         * Check c++ documentation for more information. 
         */
        std::stringstream vertexStream, fragmentStream;
        vertexStream << vShaderFile.rdbuf();
        fragmentStream << fShaderFile.rdbuf();

        // Now that we have the data, close the files.
        vShaderFile.close();
        fShaderFile.close();

        // Remember that stringstreams are not the type string. It's a stream.
        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    } 
    catch (std::ifstream::failure e) 
    {
        std::cout << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY" << std::endl;
    }

    // Recall that our source shaders were of type char and not string.
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compiling time! Process is the exact same from hello_triangle.cpp
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(vertex);
    glCompileShader(fragment);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    // Recall that the shaders are basically in C. So no strings or boolean types => cast.
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
#endif 
