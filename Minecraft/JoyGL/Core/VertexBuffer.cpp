#include "VertexBuffer.h"
#include <glad/glad.h>

namespace JoyGL {
	VertexBuffer::VertexBuffer(float* vertices, unsigned int size)
		: m_Size(size) {
		glCreateBuffers(1, &m_GLID);
		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
		glBufferData(GL_ARRAY_BUFFER, m_Size, vertices, GL_STATIC_DRAW);
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_GLID);
	}
	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
	}
	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}