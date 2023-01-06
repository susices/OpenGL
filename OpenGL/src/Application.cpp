
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream ifstream(filePath);

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

static unsigned int CompileShader( unsigned int type,const std::string& source)
{
    unsigned int id =  glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);

    //wenchao Error handling
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
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
    GLFWwindow* window;  
    
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit()!= GLEW_OK)
    {
        std::cout<< "glewInit error!"<<std::endl;
    }

    std::cout<< glGetString(GL_VERSION)<<std::endl;

    float positions[6] =
        {
        -0.5f,-0.5f,
        0.0f,0.5f,
        0.5,0.0f,
        // -0.5f,-0.5f,
        // 0.0f,-0.5f,
        // 0.5,0.0f,
        };

    unsigned int buffer ;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions),positions,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float) *2,0);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shaderfile");
    // std::cout<<"Vertex"<<std::endl;
    // std::cout<<source.VertexSource<<std::endl;
    // std::cout<<"Fragment"<<std::endl;
    // std::cout<<source.FragmentSource<<std::endl;
    
    unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
    glUseProgram(shader);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0,3);
        //glDrawElements(GL_TRIANGLES,3)
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}