#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak(); // __debugbreak is msvc specific
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))    

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
