#include "Bufferlayout.h"
#include <GL/glew.h>


void BufferLayout::GenerateLayout(int enabledID, const int bufferID, int bufferType)
{
    glEnableVertexAttribArray(enabledID);
    glBindBuffer(bufferType, bufferID);
    
    glVertexAttribPointer(
        enabledID,                        // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
}
