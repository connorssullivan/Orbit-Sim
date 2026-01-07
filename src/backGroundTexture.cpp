#include "backGroundTexture.h"


BackgroundTexture::BackgroundTexture(const char* path) : m_textureID(0), m_width(0), m_height(0), m_nrChannels(0)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);

    if (!data)
    {
        std::cerr << "Failed to load texture: " << path << "\n";
        return;
    }

    GLCall(glGenTextures(1, &m_textureID));
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    //Set the wrapping/filtering options (On currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (m_nrChannels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

BackgroundTexture::~BackgroundTexture()
{
    glDeleteTextures(1, &m_textureID);
}


void BackgroundTexture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void BackgroundTexture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}