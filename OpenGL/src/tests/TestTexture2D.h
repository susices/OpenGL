#pragma once
#include "Test.h"
#include "../Texture.h"
#include "glm/glm.hpp"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

namespace test
{
    class TestTexture2D : public test::Test
    {
    public:
        TestTexture2D();
        ~TestTexture2D();

        void OnUpdate(float deltaUpdate) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec3 m_TranslationA;
        glm::vec3 m_TranslationB;

        std::unique_ptr<VertexArray>  m_VertexArray;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;

        glm::mat4 m_ProjMatrix;
        glm::mat4 m_ViewMatrix;

        
    };
}

