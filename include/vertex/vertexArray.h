#pragma once

#include "vertexBuffer.h"
#include "vertexLayout.h"

class VertexArray
{
    private:
        unsigned int m_rendererID;
    public:
        // Constuctors 
        VertexArray();
        ~VertexArray();


        // Member Functions
        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void unBind() const;

};