#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace JoyGL {

	class PerspectiveCamera {
	public:
		PerspectiveCamera();
		~PerspectiveCamera();

		void SetProjection(const glm::mat4& matrix);
		void SetView(const glm::mat4& matrix);

		inline const glm::mat4& GetProjection() const { return m_Projection; }
		inline const glm::mat4& GetView() const { return m_View; }



	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
	};
}