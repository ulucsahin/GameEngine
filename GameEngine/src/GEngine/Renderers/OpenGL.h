#pragma once
#include "Renderer.h"

class GLFWwindow;

class OpenGL :
    public Renderer
{
public:
    OpenGL();
    ~OpenGL();

    void Start() override;
    bool WindowShouldClose() override;

    GLFWwindow* window;
};

