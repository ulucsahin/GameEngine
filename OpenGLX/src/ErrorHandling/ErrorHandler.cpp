#include <GL/glew.h>
#include <iostream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
    {
        // we dont need to do anything at the moment
    }
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) // assigns return value of glGetError to "error", and while loop runs as long as "error" is not null
    {
        std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}
