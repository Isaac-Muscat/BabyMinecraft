#include "Player.h"
#include "Chunk.h"
#include "Game.h"

Player::Player(const glm::vec3& pos, const glm::vec3& direction, const glm::mat4& projection)
	: m_Pos(pos), m_PrevPos(pos), m_SpawnPos(pos), m_Projection(projection), m_Direction(direction), m_Pitch(0.0f), m_Yaw(90.0f) {
	m_View = glm::lookAt(m_Pos, m_Pos + m_Direction, glm::vec3(0, 1, 0));
}
Player::~Player()
{
}

void Player::Update(GLFWwindow* window)
{
	m_PrevPos = m_Pos;

	// Update the player position based off of key presses.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Pos += m_Speed * glm::normalize(glm::vec3(m_Direction.x, 0, m_Direction.z));
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Pos -= m_Speed * glm::normalize(glm::vec3(m_Direction.x, 0, m_Direction.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Pos -= m_Speed * glm::normalize(glm::cross(m_Direction, glm::vec3(0, 1, 0)));
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Pos += m_Speed * glm::normalize(glm::cross(m_Direction, glm::vec3(0, 1, 0)));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_Pos += m_Speed * glm::vec3(0, 1, 0);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_Pos -= m_Speed * glm::vec3(0, 1, 0);
	}

	UpdateViewMatrix();
}

void Player::CursorPosCallbackFunction(double xPos, double yPos)
{
	static bool firstMove = true;
	if (firstMove) {
		m_PrevMousePos.x = xPos;
		m_PrevMousePos.y = yPos;
		firstMove = false;
	}

	float xoffset = xPos - m_PrevMousePos.x;
	float yoffset = m_PrevMousePos.y - yPos;
	m_PrevMousePos.x = xPos;
	m_PrevMousePos.y = yPos;

	xoffset *= m_Sensitivity;
	yoffset *= m_Sensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	// Update the player view direction vector based on mouse input
	m_Direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Direction.y = sin(glm::radians(m_Pitch));
	m_Direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
}

bool Player::ChangedChunks()
{
	glm::vec3 posRelativeToStartPos = m_Pos - m_SpawnPos;
	glm::vec3 prevPosRelativeToStartPos = m_PrevPos - m_SpawnPos;
	if (floor(posRelativeToStartPos.x / Chunk::CHUNK_SIZE) != floor(prevPosRelativeToStartPos.x / Chunk::CHUNK_SIZE) ||
		floor(posRelativeToStartPos.y / Chunk::CHUNK_SIZE) != floor(prevPosRelativeToStartPos.y / Chunk::CHUNK_SIZE) ||
		floor(posRelativeToStartPos.z / Chunk::CHUNK_SIZE) != floor(prevPosRelativeToStartPos.z / Chunk::CHUNK_SIZE))
	{
		return true;
	}
	return false;
}

void Player::UpdateViewMatrix()
{
	m_View = glm::lookAt(m_Pos + m_HeadOffset, m_Pos + m_HeadOffset + m_Direction, glm::vec3(0, 1, 0));
}
