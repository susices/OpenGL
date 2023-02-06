#include "TestTexture2D.h"
#include "../Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"
#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace test
{
    TestTexture2D::TestTexture2D():  m_TranslationA(200,200,0),m_TranslationB(400,200,0),m_Shader("res/shaders/Basic.shaderfile")
    {

        
        float positions[] =
        {
            -50.0f,-50.0f,0.0f,0.0f,//0
            50.0f,-50.0f,1.0f,0.0f,//1
            50.0f,50.0f,1.0f,1.0f,//2
            -50.0f,50.0f,0.0f,1.0f,//3
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
        glm::mat4 view = glm::translate(glm::mat4 (1.0f), glm::vec3(0,0,0));
        
        m_Shader.Bind();
        m_Shader.SetUniform4f("u_Color", 0.8f,0.3f,0.8f,1.0f);


        Texture texture("res/textures/image.png");
        texture.Bind();
        m_Shader.SetUniform1i("u_Texture",0);

        // clear bind data
        vertex_array.Unbind();
        vertex_buffer.Unbind();
        indexBuffer.Unbind();
        m_Shader.Unbind();

    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaUpdate)
    {
    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT))

        Renderer renderer;
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f),m_TranslationA);

            glm::mat4 mvp = proj * view * model;

            m_Shader.SetUniformMat4f("u_MVP",mvp);
            renderer.Draw(vertex_array,indexBuffer,m_Shader);
        }
        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f),m_TranslationB);

            glm::mat4 mvp = proj * view * model;

            m_Shader.SetUniformMat4f("u_MVP",mvp);
            renderer.Draw(vertex_array,indexBuffer,m_Shader);
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("TranslationA",&m_TranslationA.x,0.0f,960.0f);

        ImGui::SliderFloat3("TranslationB",&m_TranslationB.x,0.0f,960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}

