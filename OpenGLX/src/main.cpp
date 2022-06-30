#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "tests/TestClearColor.h"

#include "ResourceHandling/ModelLoader.h"

bool enableCore = false;

// main
int main(void)
{
    GLFWwindow* window;

    const char* objFilePath = "res/models/dummy2.obj";
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec2> texture_coords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    //bool success = ModelLoader::LoadObj(objFilePath, vertices, uvs, normals, indices);
    bool success = ModelLoader::LoadObj2(objFilePath, vertices, texture_coords, indices);

   

   
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    if (enableCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // sync refresh with monitors refresh rate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // texture coordinates added
        //float positions[] = {
        // -50.0f, -50.0f, 0.0f, 0.0f, // 0
        //  50.0f, -50.0f, 1.0f, 0.0f, // 1
        //  50.0f,  50.0f, 1.0f, 1.0f, // 2
        // -50.0f,  50.0f, 0.0f, 1.0f // 3
        //};

        //// you can also use char short etc. to save memory, but it has to be unsigned
        //unsigned int indices[] = {
        //    0, 1, 2,
        //    3, 2, 0
        //};

        // enable blending for alpha channel
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        int numVertex = 4;
        int floatsPerVertex = 4;
        //VertexBuffer vb(positions, numVertex * floatsPerVertex * sizeof(float));
        VertexBuffer vb(&vertices[0], vertices.size() * sizeof(glm::vec3));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //IndexBuffer ib(indices, 6);
        unsigned int* indicesArray = &indices[0];
        IndexBuffer ib(indicesArray, indices.size());

        // MVP: model view projection matrixes
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/lmao.png");
        unsigned int slot = 0;
        texture.Bind(slot);
        shader.SetUniform1i("u_Texture", slot);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        glm::vec3 translationA(200.f, 200.f, 0.f);
        glm::vec3 translationB(400.f, 200.f, 0.f);

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */


        test::TestClearColor test;

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            test.OnUpdate(0.0f);
            test.OnRender();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); // ? 
                glm::mat4 mvp = proj * view * model; // order matters
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            shader.Bind();
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // ? 
                glm::mat4 mvp = proj * view * model; // order matters
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }


            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    // Cleanup
    glfwTerminate();
    return 0;
}
