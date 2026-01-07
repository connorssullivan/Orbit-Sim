#include "shader.h"
#include <glm/gtc/type_ptr.hpp>



Shader::Shader() 
{
    
}


void Shader::SetShader(const char* vertexPath, const char* fragPath)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Set up exceptions fo ifstream
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragPath);

        // Read the files
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        m_vertexCode = vShaderStream.str();
        m_fragmentCode = fShaderStream.str();
    }

    catch(const std::ifstream::failure e)
    {
        std::cerr << e.what() << '\n';
    }

    const char* vShaderCode = m_vertexCode.c_str();
    const char* fShaderCode = m_fragmentCode.c_str();


    // Compile shaders 
    unsigned int vertex, fragment;
    int success; 
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertex, 1, &vShaderCode, nullptr));
    GLCall(glCompileShader(vertex));

    GLCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "Error Vertex Shater Compilation: " << infoLog << "\n";
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragment, 1, &fShaderCode, nullptr));
    GLCall(glCompileShader(fragment));

    GLCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "Error Vertex Shater Compilation: " << infoLog << "\n";
    }


    // Attach Shaders
    m_shaderID = glCreateProgram();
    GLCall(glAttachShader(m_shaderID, vertex));
    GLCall(glAttachShader(m_shaderID, fragment));
    GLCall(glLinkProgram(m_shaderID));

    GLCall(glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success));
    if (!success)
    {
        glGetProgramInfoLog(m_shaderID, 512, nullptr, infoLog);
        std::cout << "Error Linking shaders: " << infoLog << "\n";
    }

    GLCall(glDeleteShader(vertex));
    GLCall(glDeleteShader(fragment));
}

void Shader::SetFloat(const std::string& name, float value) const
{
    int location = glGetUniformLocation(m_shaderID, name.c_str());
    if (location == -1) return;

    GLCall(glUniform1f(location, value)); // ✅ CORRECT
}


void Shader::SetInt(const std::string& name, int value) const
{
    int location = glGetUniformLocation(m_shaderID, name.c_str());
    if (location == -1) return;

    GLCall(glUniform1i(location, value));
}

void Shader::SetBool(const std::string& name, bool value) const
{
    int location = glGetUniformLocation(m_shaderID, name.c_str());
    if (location == -1) return;

    GLCall(glUniform1i(location, value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    int location = glGetUniformLocation(m_shaderID, name.c_str());
    
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' not found or optimized out!\n";
        return;
    }
    
    GLCall(glUniformMatrix4fv(
        location,
        1,
        GL_FALSE,
        glm::value_ptr(mat)
    ));
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
    int location = glGetUniformLocation(m_shaderID, name.c_str());

    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' not found or optimized out!\n";
        return;
    }

    GLCall(glUniform2fv(location, 1, glm::value_ptr(value)));
}


void Shader::Use()
{
    GLCall(glUseProgram(m_shaderID));
}