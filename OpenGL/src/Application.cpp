
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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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


        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window,true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();


        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f,0.55f,0.60f,1.00f);
        
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

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

            {
                if (show_demo_window)
                    ImGui::ShowDemoWindow(&show_demo_window);
            
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            
                if (show_another_window)
                {
                    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                    ImGui::Text("Hello from another window!");
                    if (ImGui::Button("Close Me"))
                        show_another_window = false;
                    ImGui::End();
                }
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
        
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 
    glfwTerminate();
    return 0;
}
