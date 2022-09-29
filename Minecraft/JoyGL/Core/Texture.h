#pragma once
#include <string>

namespace JoyGL {

	class Texture {
	public:
		Texture(const std::string& textureFilepath);
		~Texture();

		void Bind() const;

	private:
		unsigned int m_GLID;
	};
}