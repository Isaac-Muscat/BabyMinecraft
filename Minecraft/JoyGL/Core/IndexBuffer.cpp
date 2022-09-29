#include "IndexBuffer.h"
#include <glad/glad.h>

namespace JoyGL {
	IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int size)
		: m_Size(size) {
		glCreateBuffers(1, &m_GLID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, indices, GL_STATIC_DRAW);
	}
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_GLID);
	}
	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLID);
	}
	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}