#pragma once


namespace JoyGL {

	class VertexBuffer {
	public:
		VertexBuffer(float* vertices, unsigned int size);
		~VertexBuffer();

		void Bind();
		void Unbind();
	private:
		unsigned int m_GLID;
		unsigned int m_Size;
	};

	
}
