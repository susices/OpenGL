
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Shader.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tests/TestCleatColor.h"

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

        GLCall(glEnable(GL_BLEND))
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
        
        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window,true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        auto test_menu = new test::TestMenu(currentTest);
        currentTest = test_menu;

        test_menu->RegisterTest<test::TestCleatColor>("Test Color");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClearColor(0.f,0.0f,0.0f,1.0f))
            renderer.Clear();
            

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest!=nullptr)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest!= test_menu && ImGui::Button("Back"))
                {
                    delete currentTest;
                    currentTest = test_menu;
                }
                currentTest->OnImGuiRender();
                
                ImGui::End();
            }
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest!=test_menu)
        {
            delete test_menu;
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 
    glfwTerminate();
    return 0;
}
