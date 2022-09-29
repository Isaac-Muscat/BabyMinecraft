#include "VertexArray.h"
#include <glad/glad.h>

namespace JoyGL {
	VertexArray::VertexArray(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexArrayLayout> layout)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_Layout(layout) {
		glCreateVertexArrays(1, &m_GLID);
		Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();

		unsigned int byteOffset = 0;

		for (auto& element : m_Layout->GetLayout())
		{
			glVertexAttribPointer(
				element.layoutIndex, 
				element.numberOfElements, 
				element.type, 
				element.normalized, 
				m_Layout->GetStride(),
				(void*)byteOffset
			);
			glEnableVertexArrayAttrib(m_GLID, element.layoutIndex);
			byteOffset += element.numberOfElements * element.sizeOfType;
		}
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_GLID);
	}
	void VertexArray::Bind() const
	{
		glBindVertexArray(m_GLID);
	}
	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	VertexArrayLayout::VertexArrayLayout(std::vector<VertexArrayLayoutElement> layout)
		: m_Layout(layout), m_Stride(0) {
		for (auto& element : m_Layout)
		{
			m_Stride += element.sizeOfType * element.numberOfElements;
		}
	}
	VertexArrayLayout::~VertexArrayLayout()
	{
	}
}