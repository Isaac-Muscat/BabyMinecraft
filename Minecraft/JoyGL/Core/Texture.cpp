#include "Texture.h"
#include <glad/glad.h>
#include <JoyGL/stb_image/stb_image.h>

namespace JoyGL {
	Texture::Texture(const std::string& textureFilepath)
	{
		glGenTextures(1, &m_GLID);
		glBindTexture(GL_TEXTURE_2D, m_GLID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(textureFilepath.c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	Texture::~Texture()
	{
	}
	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_GLID);
	}
}