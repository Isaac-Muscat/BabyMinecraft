#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace JoyGL {

	class Shader {
	public:
		Shader(const char* vertexSource, const char* fragSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMatrix4fv(const std::string& uniformName, const glm::f32* matrix);

		inline unsigned int GetGLID() { return m_GLID; }

		static std::string LoadShaderString(const std::string& filePath);
	private:
		unsigned int m_GLID;
		std::unordered_map<std::string, unsigned int> m_UniformMap;
	private:
	};
}