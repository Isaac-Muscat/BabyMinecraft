#pragma once

namespace JoyGL {

	class IndexBuffer {
	public:
		IndexBuffer(unsigned int* indices, unsigned int size);
		~IndexBuffer();

		void Bind();
		void Unbind();
	private:
		unsigned int m_GLID;
		unsigned int m_Size;
	};
}
