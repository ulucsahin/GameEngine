#include "OpenGL.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image/stb_image.h"
#include "GEngine/ErrorHandling/ErrorHandler.h"

OpenGL::OpenGL()
{

}

OpenGL::~OpenGL()
{
    if (window)
        delete window;
}

void OpenGL::Start()
{
    bool enableCore = false;

    stbi_set_flip_vertically_on_load(1);

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(-1);
    }

    if (enableCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    // Create window 
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. \n");
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // sync refresh with monitors refresh rate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW ERROR!" << std::endl;
        exit(-1);
    }

    // enable blending for alpha channel
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    std::cout << glGetString(GL_VERSION) << std::endl;
}

bool OpenGL::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}
