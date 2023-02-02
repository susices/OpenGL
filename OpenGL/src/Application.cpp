
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
int main(void)
{
    GLFWwindow* window;  
    
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit()!= GLEW_OK)
    {
        std::cout<< "glewInit error!"<<std::endl;
    }

    std::cout<< glGetString(GL_VERSION)<<std::endl;

    {
        float positions[] =
            {
            100.0f,100.0f,0.0f,0.0f,//0
            200.0f,100.0f,1.0f,0.0f,//1
            200.0f,200.0f,1.0f,1.0f,//2
            100.0f,200.0f,0.0f,1.0f,//3
            };

        unsigned int indices[] = {
            0,1,2,
            2,3,0,
        };
        GLCall(glEnable(GL_BLEND))
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
        

        VertexArray vertex_array;
        VertexBuffer vertex_buffer(positions, 4*4*sizeof(float));
        VertexBufferLayout vertex_buffer_layout;
        vertex_buffer_layout.Push<float>(2);
        vertex_buffer_layout.Push<float>(2);
        vertex_array.AddBuffer(vertex_buffer, vertex_buffer_layout);

        IndexBuffer indexBuffer(indices,6);

        glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
        glm::mat4 view = glm::translate(glm::mat4 (1.0f), glm::vec3(-100,0,0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(200,200,0));

        glm::mat4 mvp = proj * view * model;
        
        Shader shader("res/shaders/Basic.shaderfile");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f,0.3f,0.8f,1.0f);
        shader.SetUniformMat4f("u_MVP",mvp);

        Texture texture("res/textures/image.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture",0);

        // clear bind data
        vertex_array.Unbind();
        vertex_buffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();

        Renderer renderer;
        
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r,0.3f,0.8f,1.0f);
            
            renderer.Draw(vertex_array,indexBuffer,shader);

            if (r >1.0f)
            {
                increment = -0.05f;
            }else if (r < 0.0f)
            {
                increment = 0.05f;
            }

            r += increment;
        
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
