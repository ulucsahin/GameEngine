#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		unsigned int stride = layout.GetStride();
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset));  // timed link: https://youtu.be/x0H--CL2tUI?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&t=782
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}


void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}


void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

