#include "PerspectiveCamera.h"

namespace JoyGL {
	PerspectiveCamera::PerspectiveCamera()
		: m_View(1.0f), m_Projection(1.0f) {
	}
	PerspectiveCamera::~PerspectiveCamera()
	{
	}
	void PerspectiveCamera::SetProjection(const glm::mat4& matrix)
	{
		m_Projection = matrix;
	}
	void PerspectiveCamera::SetView(const glm::mat4& matrix)
	{
		m_View = matrix;
	}
}