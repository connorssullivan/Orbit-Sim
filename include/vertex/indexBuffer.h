#include <renderer.h>

class IndexBuffer
{
    private:
        unsigned int m_rendererID;
        unsigned int m_count;
    public:
        // Constructors 
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        // Getters
        unsigned int GetCount() const {return m_count;}


        // Member functions
        void Bind() const;
        void Unbind() const;

};