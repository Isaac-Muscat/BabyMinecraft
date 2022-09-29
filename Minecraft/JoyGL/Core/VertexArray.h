#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>
#include <vector>

namespace JoyGL {

	struct VertexArrayLayoutElement {
		unsigned int layoutIndex;
		int numberOfElements;
		unsigned int type;
		unsigned char normalized;
		int sizeOfType;
	};

	class VertexArrayLayout {
	public:
		VertexArrayLayout(std::vector<VertexArrayLayoutElement> layout);
		~VertexArrayLayout();

		inline int GetStride() const { return m_Stride; }
		inline std::vector<VertexArrayLayoutElement>& GetLayout() { return m_Layout; }

	private:
		int m_Stride;
		std::vector<VertexArrayLayoutElement> m_Layout;
	};

	class VertexArray {
	public:
		VertexArray(
			std::shared_ptr<VertexBuffer> vertexBuffer, 
			std::shared_ptr<IndexBuffer> indexBuffer,
			std::shared_ptr<VertexArrayLayout> layout);
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetGLID() { return m_GLID; }

	private:
		unsigned int m_GLID;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArrayLayout> m_Layout;
	};
}
