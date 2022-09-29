#include "Game.h"
#include "WorldGenerator.h"
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(int width, int height, const std::string& title)
	: m_Width(width), m_Height(height), m_Title(title)
{
	// Initialization.
	if (!glfwInit()) std::cerr << "Could not load glfw!\n";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
	}

	glfwSwapInterval(1);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_Window, this);
	
	// Setup input callbacks
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
	{
		Game* game = (Game*) glfwGetWindowUserPointer(window);
		game->CursorPosCallbackFunction(xPos, yPos);
	});

	// Setup player and world
	m_Cursor = std::make_unique<Cursor>();
	m_Player = std::make_unique<Player>(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f)
	);
	m_World = std::make_unique<World>(m_Player->GetPosition());

	// Configure the world generator.
	WorldGenerator::ConfigureNoise();

}

Game::~Game()
{
}

void Game::Run()
{

	// Setup.
	glEnable(GL_CULL_FACE);

	// Main loop.
	while (!glfwWindowShouldClose(m_Window)) {
		// Check escape key.
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}
		else if (glfwGetKey(m_Window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		// Clear buffers.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.8f, 1.0f);

		// Update game state.
		Update();

		// Draw the world.
		Draw();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Game::CursorPosCallbackFunction(double xPos, double yPos)
{
	m_Player->CursorPosCallbackFunction(xPos, yPos);
}

void Game::Update()
{
	m_Player->Update(m_Window);
	m_World->Update(m_Player->GetPosition());

	if (m_Player->ChangedChunks()) {
		// std::cout << "Player pos: " << m_Player->GetPosition().x << " " << m_Player->GetPosition().y << " "  << m_Player->GetPosition().z << "\n";
		m_World->UpdateChunksFromPlayerPos(m_Player->GetPosition(), m_Player->GetPreviousPosition());
	}
}

void Game::Draw() const
{
	m_World->Draw(m_Player->GetProjectionMatrix(), m_Player->GetViewMatrix());
	//m_Cursor->Draw();
}
