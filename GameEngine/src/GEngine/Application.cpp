#include "Application.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ResourceHandling/ModelLoader.h"
#include "ResourceHandling/Texture.h"
#include "ErrorHandling/ErrorHandler.h"
#include "Shaders/ShaderHandler.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "Translation.h"
#include "stb_image/stb_image.h"
#include <numbers>
#include <cmath>
#include <filesystem>
#include "Renderers/Renderer.h"
#include "Renderers/OpenGL.h"

void SpawnModel(const char* modelPath, const char* texturePath = nullptr)
{

}

namespace GEngine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
        OpenGL renderer = OpenGL();
        renderer.Start();

        VertexBuffer bufferManager;
        BufferLayout bufferLayout;

        // VAO
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Load Model 
        std::vector<GLfloat> vertices;
        std::vector<int> indexes;
        std::vector<float> UV;
        vertices.reserve(50000); // TODO: don't use vectors
        ModelLoader::LoadModel3("res/models/dummy2.obj", vertices, indexes, UV);

        // Vertex
        GLuint vertexbuffer;
        static const GLfloat* g_vertex_buffer_data = &vertices[0];
        bufferManager.GenerateBuffer(vertexbuffer, g_vertex_buffer_data, vertices.size() * sizeof(GLfloat), GL_ARRAY_BUFFER);

        // Index
        GLuint indexBuffer;
        static const int* g_index_buffer_data = &indexes[0];
        bufferManager.GenerateBufferi(indexBuffer, g_index_buffer_data, indexes.size() * sizeof(GLfloat), GL_ELEMENT_ARRAY_BUFFER);

        // UV
        GLuint uvBuffer;
        static const GLfloat* g_uv_buffer_data = &UV[0];
        bufferManager.GenerateBuffer(uvBuffer, g_uv_buffer_data, UV.size() * sizeof(GLfloat), GL_ARRAY_BUFFER);

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        GLfloat FoV = 90.f;
        GLfloat aspectRatio = 4.f / 3.f;
        GLfloat near = 0.1f;
        GLfloat far = 100.f;
        glm::mat4 Projection = glm::perspective(glm::radians(FoV), aspectRatio, near, far);

        // Camera matrix
        glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, -3), // Camera is at (4, 3, 3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0, -1, 0 to look upside-down)
        );

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);

        // load shaders
        ShaderHandler shaderHandler;
        unsigned int ProgramID = shaderHandler.GetProgramID();
        ShaderInfo vertexShader = shaderHandler.LoadShader("res/shaders/VertexShader.shader", GL_VERTEX_SHADER);
        ShaderInfo fragmentShader = shaderHandler.LoadShader("res/shaders/FragmentShader.shader", GL_FRAGMENT_SHADER);

        // Compile Shaders
        shaderHandler.CompileAttachShader(vertexShader);
        shaderHandler.CompileAttachShader(fragmentShader);

        // Link program
        GLCall(glLinkProgram(ProgramID));

        // Detach shaders
        GLCall(glDetachShader(ProgramID, vertexShader.ID));
        GLCall(glDetachShader(ProgramID, fragmentShader.ID));
        GLCall(glDeleteShader(vertexShader.ID));
        GLCall(glDeleteShader(fragmentShader.ID));

        // Texture
        Texture texture("res/textures/lmao.png");
        texture.Bind();

        // Get a handle for our "MVP" uniform
        // Only during the initialisation
        GLuint MVPID = glGetUniformLocation(ProgramID, "MVP");

        GLCall(glEnable(GL_DEPTH_TEST)); // Enable depth test
        GLCall(glDepthFunc(GL_LESS)); // Accept fragment if it closer to the camera than the former one
        glEnable(GL_CULL_FACE);
            
        double currentTime = 0.0;
        double lastTime = 0.0;
        while (!renderer.WindowShouldClose())
        {
            currentTime = glfwGetTime();
            float deltaTime = float(currentTime - lastTime);
            lastTime = currentTime;

            double xpos, ypos;
            glfwGetCursorPos(renderer.window, &xpos, &ypos);

            Translation::Rotate(Model, { 0.1f, 0.1f, 0.1f });

            glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            GLCall(glUseProgram(ProgramID));

            bufferLayout.GenerateLayout(0, vertexbuffer, GL_ARRAY_BUFFER);
            bufferLayout.GenerateLayout(1, uvBuffer, GL_TEXTURE_BUFFER);

            // Send our transformation to the currently bound shader, in the "MVP" uniform
            // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
            GLCall(glUniformMatrix4fv(MVPID, 1, GL_FALSE, &mvp[0][0]));

            // draw call
            // GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices.size())); // without indexes
            GLCall(glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, nullptr)); // with indexes
            GLCall(glDisableVertexAttribArray(0));

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(renderer.window));

            ///* Poll for and process events */
            GLCall(glfwPollEvents());
            
        }

        // Cleanup
        GLCall(glfwTerminate());
        exit(0);
	}

}