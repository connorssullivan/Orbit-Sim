#pragma once

#include <renderer.h>

class VertexBuffer
{
    private:
        unsigned int m_rendererID;
    public:
        // Constructors 
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();


        // Member functions
        void Bind() const;
        void Unbind() const;
};