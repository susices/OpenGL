#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#include "Renderer.h"

Shader::Shader(const std::string& filePath): m_FilePath(filePath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader();
    
    m_RendererID = CreateShader(source.VertexSource,source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID))
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID))
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0))
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name),value))
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name),value))
}

void Shader::SetUniform4f(const std::string& name,float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name),v0,v1,v2,v3))
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,&matrix[0][0]));
}

unsigned Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name)!=m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    GLCall(int location =  glGetUniformLocation(m_RendererID, name.c_str()))
    
    if (location==-1)
    {
        std::cout<< "Warning: uniform '"<<name<<"' doesn't exist"<<std::endl;
    }
    
    m_UniformLocationCache[name] = location;
    
    return location;
}


ShaderProgramSource Shader::ParseShader()
{
    std::ifstream ifstream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    std::string line;
    std::stringstream sstream [2];
    ShaderType shaderType = ShaderType::NONE;
    while (std::getline(ifstream,line))
    {
        if (line.find("#shader")!=std::string::npos)
        {
            if (line.find("vertex")!=std::string::npos)
            {
                shaderType = ShaderType::VERTEX;
                
            }else if(line.find("fragment")!=std::string::npos)
            {
                shaderType = ShaderType::FRAGMENT;
            }
        }else
        {
            sstream[(int) shaderType]<<line << '\n';
        }
    }
    return {sstream[0].str(),sstream[1].str()};
}

unsigned int Shader::CompileShader( unsigned int type,const std::string& source)
{
    unsigned int id =  glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH,&length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id,length,&length,message);
        std::string shaderType = (type== GL_VERTEX_SHADER? "Vertex":"Fragment");
        
        std::cout<< "Failed to compile"<< shaderType <<" shader!"<<std::endl;
        std::cout<< message<<std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program =  glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




