#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace JoyGL {

	class Window {
	public:
		Window(unsigned int width, unsigned int height, const std::string& title);
		~Window();

		inline GLFWwindow* GetNativeWindowPointer() { return m_Window; }

	private:
		unsigned int m_Width;
		unsigned int m_Height;
		std::string m_Title;

		GLFWwindow* m_Window;
	};
}