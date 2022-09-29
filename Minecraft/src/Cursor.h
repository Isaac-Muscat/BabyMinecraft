#pragma once
#include "JoyGL/Core/Shader.h"
#include <memory>

class Cursor
{
public:
	Cursor();

	void Draw();

private:
	unsigned int m_GLVBOID;
	unsigned int m_GLVBAID;
	unsigned int m_GLTextureID;

	std::unique_ptr<JoyGL::Shader> m_TextureShader;
};

