#pragma once
#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:return sizeof(float);
        case GL_UNSIGNED_INT: return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
        default: ASSERT(false) return 0;
        }
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout():m_Stride(0){};
    ~VertexBufferLayout(){};

    template<typename T>
    void Push(unsigned int count)
    {
        ASSERT(true);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT,count,GL_FALSE});
        m_Stride+= VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT,count,GL_FALSE});
        m_Stride+=VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE,count,GL_TRUE});
        m_Stride+=VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElements() const {return m_Elements;}
    inline unsigned int GetStride()const{return m_Stride;}
    
};
