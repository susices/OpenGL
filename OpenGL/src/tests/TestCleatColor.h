#pragma once
#include "Test.h"

namespace test
{
    class TestCleatColor: public Test
    {
    public:
        TestCleatColor();
        ~TestCleatColor();

        void OnUpdate(float deltaUpdate) override;
        void OnRender() override;
        void OnImGuiRender() override;

        
    };
}

