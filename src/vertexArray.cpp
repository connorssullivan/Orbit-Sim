#include "vertexArray.h"



VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_rendererID);
    glBindVertexArray(m_rendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_rendererID);
}


        
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto& elements = layout.getElements();
    unsigned int offset {};

    for (unsigned int i {0}; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.m_count, element.m_type, element.m_normalized, 
                            layout.getStride(), (void*)(size_t)offset);
        
        offset += element.getSizeOfType(element.m_type) * element.m_count;
    }
}


void VertexArray::Bind() const
{
    glBindVertexArray(m_rendererID);
}

void VertexArray::unBind() const
{
    glBindVertexArray(0);
}