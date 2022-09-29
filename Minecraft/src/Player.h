#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Player {
public:
	Player(const glm::vec3& pos, const glm::vec3& direction, const glm::mat4& projection);
	~Player();

	void Update(GLFWwindow* window);
	void CursorPosCallbackFunction(double xPos, double yPos);
	bool ChangedChunks();

	inline const glm::vec3& GetPosition() const { return m_Pos; }
	inline const glm::vec3& GetPreviousPosition() const { return m_PrevPos; }
	inline const glm::vec3& GetSpawnPosition() const { return m_SpawnPos; }

	inline const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
	inline const glm::mat4& GetViewMatrix() const { return m_View; }

private:
	void UpdateViewMatrix();



private:
	float m_Yaw, m_Pitch;
	const float m_Speed = 0.1f;
	glm::mat4 m_Projection, m_View;
	glm::vec3 m_Pos, m_PrevPos, m_Direction;
	const glm::vec3 m_HeadOffset = glm::vec3(0, 2, 0);
	const glm::vec3 m_SpawnPos;
	float m_Sensitivity = 0.1f;

	// Variables for handling inputs.
	glm::vec2 m_PrevMousePos;
};
