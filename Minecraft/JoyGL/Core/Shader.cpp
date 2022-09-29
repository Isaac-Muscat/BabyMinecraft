#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Shader.h"


namespace JoyGL {
	Shader::Shader(const char* vertexChar, const char* fragChar)
	{
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexChar, 0);
		glCompileShader(vertexShader);

		int success = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(vertexShader);
			std::cerr << errorLog.data();
		}
		unsigned int fragShader;
		fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fragChar, 0);
		glCompileShader(fragShader);

		success = 0;
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(fragShader);
			std::cerr << errorLog.data();
		}

		m_GLID = glCreateProgram();
		glAttachShader(m_GLID, vertexShader);
		glAttachShader(m_GLID, fragShader);

		glLinkProgram(m_GLID);

		GLint isLinked = 0;
		glGetProgramiv(m_GLID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_GLID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_GLID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shaderProgram anymore.
			glDeleteProgram(m_GLID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragShader);

			// Use the infoLog as you see fit.

			std::cerr << infoLog.data();
			// In this simple shaderProgram, we'll just leave
		}


		glDetachShader(m_GLID, vertexShader);
		glDetachShader(m_GLID, fragShader);
	}
	Shader::~Shader()
	{
	}
	void Shader::Bind() const
	{
		glUseProgram(m_GLID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMatrix4fv(const std::string& uniformName, const glm::f32* matrix)
	{
		if (m_UniformMap.find(uniformName) == m_UniformMap.end()) {
			m_UniformMap[uniformName] = glGetUniformLocation(m_GLID, uniformName.c_str());
		}
		glUniformMatrix4fv(m_UniformMap[uniformName], 1, GL_FALSE, matrix);
	}

	std::string Shader::LoadShaderString(const std::string& filePath)
	{
		std::ifstream shaderSource(filePath);
		if (!shaderSource.is_open()) {
			std::cerr << "Failed to read file: " << filePath << "\n";
		}
		std::stringstream ss;

		ss << shaderSource.rdbuf();

		shaderSource.close();
		return ss.str();
	}
}