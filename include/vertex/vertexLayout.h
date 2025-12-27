#pragma once

#include <vector>
#include <glad/glad.h>

#include "vertexBuffer.h"


struct VertexBufferElement
{
    unsigned int m_type;
    unsigned int m_count;
    bool m_normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
            break;
        case GL_UNSIGNED_INT:
            return 4;
            break;
        case GL_UNSIGNED_BYTE:
            return 1;
            break;
        
        default:
            return 0;
            break;
        }
    }
};

class VertexBufferLayout
{
    private:
        std::vector<VertexBufferElement> m_elements;
        unsigned int m_stride;

    public:
    // constructors
        VertexBufferLayout() : m_stride(0) 
        {}

        // Getters
        const std::vector<VertexBufferElement>& getElements() const {return m_elements;}
        unsigned int getStride() const {return m_stride;}

        // Member functions 
        template<typename T>
        void push(unsigned int count)
        {
            static_assert(false, "Vertax Attribute: Invalid Type");
        }
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
