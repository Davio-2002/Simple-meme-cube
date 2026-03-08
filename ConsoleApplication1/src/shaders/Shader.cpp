#include <shaders/Shader.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>

namespace {

constexpr int BUFFER_SIZE = 1024;
const std::string vertexStr = "VERTEX";
const std::string fragmentStr = "FRAGMENT";

} // anonymous namespace

Shader::Shader(const std::string& vertexPath,
               const std::string& fragmentPath) : ID{ 0 } 
{
    // Get vertex shader source code from `filePath`
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    std::string vertexCode;
    std::string fragmentCode;

    // Make sure ifstream can throw exceptions
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);

    try 
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // Write data to threads
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close files
        vShaderFile.close();
        fShaderFile.close();

        // Convert streams to strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) { 
        std::cout << e.what() << '\n';
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << '\n';
    }

    uint32_t vertex{};
    uint32_t fragment{};

    InitializeShader(vertex, GL_VERTEX_SHADER, vertexCode.c_str(), vertexStr);
    InitializeShader(fragment, GL_FRAGMENT_SHADER, fragmentCode.c_str(), fragmentStr);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkProgramForLinkErrors(ID);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
} // anonymous namespace

void Shader::checkForShaderCompileErrors(const uint32_t& shader, 
                                         const std::string& type)
{
    int success;
    char infoLog[BUFFER_SIZE];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
            << infoLog << "\n";
    }
}

void Shader::checkProgramForLinkErrors(const uint32_t& program)
{
    int success{};
    char infoLog[BUFFER_SIZE];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, BUFFER_SIZE, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n"
            << infoLog << "\n" << '\n';
    }
}

void Shader::InitializeShader(uint32_t& shader, 
                              const uint32_t type, 
                              const char* code, 
                              const std::string& errorTypeStr)
{
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    checkForShaderCompileErrors(shader, errorTypeStr);
}

void Shader::Use() const 
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float value) const 
{
    const int loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
        std::cerr << "Uniform " << name << " not found!\n";
        return;
    }
    glUniform1f(loc, value); 
}

