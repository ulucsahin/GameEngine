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

bool enableCore = false;

// main
int main(void)
{
    stbi_set_flip_vertically_on_load(1);

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    if (enableCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    } 
    
    // Create window 
     GLFWwindow* window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL) 
    {
        fprintf(stderr, "Failed to open GLFW window. \n");
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
        VertexBuffer bufferManager;
        BufferLayout bufferLayout;

        // enable blending for alpha channel
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VAO
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Load Model 
        std::vector<GLfloat> vertices;
        std::vector<int> indexes;
        std::vector<float> UV;
        vertices.reserve(50000); // todo: don't use vectors
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
        
        // rotation example
        /*float rotation = 30.f;
        float cosTheta = std::cos(rotation * toRadians);
        float sinTheta = std::sin(rotation * toRadians);
        float rot[16] = {
            1,     0,        0,     0,
            0, cosTheta, -sinTheta, 0,
            0, sinTheta, cosTheta,  0,
            0,    0,        0,      1
        };

        glm::mat4 rotationMatrix = glm::make_mat4(rot);
        std::cout << cosTheta << std::endl;*/

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        GLfloat FoV = 60.f;
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
        
        //Translation::Rotate(Model, {30.f, 30.f, 30.f});
        ////Model = Model * rotationMatrix;
        //// Our ModelViewProjection : multiplication of our 3 matrices
        //glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around


        

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

        // Enable depth test
        GLCall(glEnable(GL_DEPTH_TEST));
        // Accept fragment if it closer to the camera than the former one
        GLCall(glDepthFunc(GL_LESS));
        glEnable(GL_CULL_FACE);
        
        double currentTime = 0.0;
        double lastTime = 0.0;
        while (!glfwWindowShouldClose(window))
        {
            currentTime = glfwGetTime();
            float deltaTime = float(currentTime - lastTime);
            lastTime = currentTime; 
            //std::cout << "deltaTime: " << deltaTime << std::endl;

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            Translation::Rotate(Model, { 0.1f, 0.1f, 0.1f });
            //Model = Model * rotationMatrix;
            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

            //std::cout << xpos << " " << ypos << std::endl;

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
            GLCall(glfwSwapBuffers(window));

            ///* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    // Cleanup
    GLCall(glfwTerminate());
    return 0;
}
