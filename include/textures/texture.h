#include <string_view>
#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "renderer.h"


namespace Textures
{
    constexpr const char* EARTH_TEX = "../include/textures/earthTexture.png";
    constexpr const char* STARS_TEX = "../include/textures/starsBG.png";
};


class Texture
{
    private:
        unsigned int m_textureID;
        int m_width, m_height, m_nrChannels;
    public:

        // Constructors
        explicit Texture(const char* path);
        ~Texture();

        // Getters
        int GetWidth() const { return m_width;};
        int GetHeight() const { return m_height;};

        // Member Functions
        void Bind(unsigned int slot) const;
        void UnBind() const;


};
