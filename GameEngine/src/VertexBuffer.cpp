#include "VertexBuffer.h"
#include <GL/glew.h>


void VertexBuffer::GenerateBuffer(unsigned int& id, const float* data, int size, int type)
{
    glGenBuffers(1, &id); // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glBindBuffer(type, id); // The following commands will talk about our 'vertexbuffer' buffer
    glBufferData(type, size, data, GL_STATIC_DRAW); // Give our vertices to OpenGL.
}

void VertexBuffer::GenerateBufferi(unsigned int& id, const int* data, int size, int type)
{
    glGenBuffers(1, &id); // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glBindBuffer(type, id); // The following commands will talk about our 'vertexbuffer' buffer
    glBufferData(type, size, data, GL_STATIC_DRAW); // Give our vertices to OpenGL.
}
