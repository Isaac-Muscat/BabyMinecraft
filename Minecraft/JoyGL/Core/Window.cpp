#include "Window.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

namespace JoyGL {
	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		: m_Width(width), m_Height(height), m_Title(title) {
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
	}
	Window::~Window()
	{
	}
}