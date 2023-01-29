#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path):m_RendererID(0),m_FilePath(path),m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(),&m_Width, &m_Height,&m_BPP,4);
    GLCall(glGenTextures(1,&m_RendererID))
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID))

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR))

}

Texture::~Texture()
{
    
}

void Texture::Bind(unsigned slot) const
{
    
}

void Texture::Unbind()
{
    
}
