#pragma once
#include "Test.h"
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

        VertexArray m_VertexArray;
        IndexBuffer m_IndexBuffer;
        Shader m_Shader;

        
    };
}

