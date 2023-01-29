#include "VertexArray.h"


VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID))
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1,&m_RendererID))
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout)
{
    Bind();
    vertexBuffer.Bind();
    const auto& elements =  vertexBufferLayout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size();i++)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i))
        GLCall(glVertexAttribPointer(i,element.count,element.type,element.normalized ,vertexBufferLayout.GetStride(), (const void*)offset))
        offset+=element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID))
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0))
}
