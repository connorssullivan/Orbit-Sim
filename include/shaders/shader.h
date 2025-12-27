#pragma once 

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>

#include <glm/glm.hpp>

#include "renderer.h"

namespace ShaderPaths
{
    inline constexpr const char* VERTEX_SHADER_PATH = "../recources/vertexShader.txt";
    inline constexpr const char* FRAGMENT_SHADER_PATH = "../recources/fragShader.txt";

    inline constexpr const char* BG_FRAFMENT_SHADER_PATH = "../recources/backGroundFragShader.txt";
    inline constexpr const char* BG_VERTEX_SHADER_PATH = "../recources/backGroundVertexShader.txt";
};

class Shader
{
    private:
        unsigned int m_shaderID;
        std::string m_vertexCode;
        std::string m_fragmentCode;
    public:
        // Constructors
        Shader();

        // Setters
        void SetShader(const char* vertexPath, const char* fragPath);
        void SetFloat(const std::string& name, float value) const;
        void SetInt(const std::string& name, int value) const;
        void SetBool(const std::string& name, bool value) const;
        void SetMat4(const std::string& name, const glm::mat4& mat) const;
        void SetVec2(const std::string& name, const glm::vec2& value) const;
        



        // Member Functions
        void Use();
};