#include "Cursor.h"
#include "JoyGL/Core/Shader.h"

#include "glad/glad.h"
#include <JoyGL/stb_image/stb_image.h>

#include <memory>

Cursor::Cursor()
{
	std::string vertexString = JoyGL::Shader::LoadShaderString("resources/Shaders/Basic.vert");
	std::string fragString = JoyGL::Shader::LoadShaderString("resources/Shaders/Basic.frag");
	m_TextureShader = std::make_unique<JoyGL::Shader>(vertexString.c_str(), fragString.c_str());

	// Create Texture
	glGenTextures(1, &m_GLTextureID);
	glBindTexture(GL_TEXTURE_2D, m_GLTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// Initialize vertex array
	glCreateVertexArrays(1, &m_GLVBAID);
	glBindVertexArray(m_GLVBAID);

	float vertices[6 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,	0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
	};

	// Initialize vertex buffer
	glCreateBuffers(1, &m_GLVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, m_GLVBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setup vertex array attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexArrayAttrib(m_GLVBAID, 0);
	glEnableVertexArrayAttrib(m_GLVBAID, 1);

	// Unbind vertex array
	glBindVertexArray(0);

}

void Cursor::Draw()
{
	glDisable(GL_DEPTH_TEST);
	m_TextureShader->Bind();
	glBindTexture(GL_TEXTURE_2D, m_GLTextureID);

	glm::mat4 o = glm::ortho(0, 1800, 0, 1000);
	glm::mat4 m = glm::mat4(5);
	m_TextureShader->UploadUniformMatrix4fv("u_Projection", glm::value_ptr(o));
	m_TextureShader->UploadUniformMatrix4fv("u_Model", glm::value_ptr(m));

	glBindVertexArray(m_GLVBAID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
