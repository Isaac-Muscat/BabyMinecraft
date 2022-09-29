//#include "pch.h"
//#include <GLFW/glfw3.h>
//#include <glad/glad.h>
//#include <JoyGL/stb_image/stb_image.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "JoyGL/Core/Shader.h"
//#include "JoyGL/Core/Window.h"
//#include "JoyGL/Core/Texture.h"
//#include "JoyGL/Core/VertexArray.h"
//#include "JoyGL/Core/VertexBuffer.h"
//#include "JoyGL/Core/IndexBuffer.h"
//#include "JoyGL/Core/PerspectiveCamera.h"
//
//
//int main(void) {
//
//	JoyGL::Window window(1600, 900, "Minecraft");
//
//	// Create Shaders
//	std::string vertexString = JoyGL::Shader::LoadShaderString("resources/Shaders/Basic.vert");
//	std::string fragString = JoyGL::Shader::LoadShaderString("resources/Shaders/Basic.frag");
//	std::shared_ptr<JoyGL::Shader> blockShader = std::make_shared<JoyGL::Shader>(vertexString.c_str(), fragString.c_str());
//
//	// Create Texutre
//	JoyGL::Texture texture("resources/Textures/JoyMinecraftTextureAtlas.png");
//
//	// Create Geometry
//	float scale = 16.0f / 256.0f;
//	float x = 1.0f;
//	float y = 15.0f;
//	float xt = 2.0f;
//	float yt = 15.0f;
//	float xb = 0.0f;
//	float yb = 15.0f;
//
//	float vertices[24 * 5] = {
//
//		-0.5f, -0.5f,  0.5f,	(x + 1) * scale, y * scale,		// Bottom left
//		 0.5f, -0.5f,  0.5f,	x * scale, y * scale,			// Bottom right
//		 0.5f,	0.5f,  0.5f,	x * scale, (y + 1) * scale,		// Top right
//		-0.5f,	0.5f,  0.5f,	(x + 1) * scale, (y + 1) * scale,// Top left
//
//		-0.5f,	0.5f,  0.5f,	(xt + 1) * scale, yt * scale,		// Top left
//		 0.5f,	0.5f,  0.5f,	xt * scale, yt * scale,			// Top right
//		 0.5f,	0.5f, -0.5f,	xt * scale, (yt + 1) * scale,		// Back Top right
//		-0.5f,	0.5f, -0.5f,	(xt + 1) * scale, (yt + 1) * scale,// Back Top left
//
//		-0.5f, -0.5f, -0.5f,	(x + 1) * scale, y * scale,		// Back Bottom left
//		-0.5f, -0.5f,  0.5f,	x * scale, y * scale,				// Bottom left
//		-0.5f,	0.5f,  0.5f,	x * scale, (y + 1) * scale,		// Top left
//		-0.5f,	0.5f, -0.5f,	(x + 1) * scale, (y + 1) * scale,	// Back Top left
//
//		 0.5f, -0.5f,  0.5f,	(x + 1) * scale, y * scale,				// Bottom right
//		 0.5f, -0.5f, -0.5f,	x * scale, y * scale,		// Back Bottom right
//		 0.5f,	0.5f, -0.5f,	x * scale, (y + 1) * scale,	// Back Top right
//		 0.5f,	0.5f,  0.5f,	(x + 1) * scale, (y + 1) * scale,		// Top right
//
//		 0.5f, -0.5f, -0.5f,	(x + 1) * scale, y * scale,		// Back Bottom right
//		-0.5f, -0.5f, -0.5f,	x * scale, y * scale,			// Back Bottom left
//		-0.5f,	0.5f, -0.5f,	x * scale, (y + 1) * scale,		// Back Top left
//		 0.5f,	0.5f, -0.5f,	(x + 1) * scale, (y + 1) * scale,// Back Top right
//
//		 0.5f, -0.5f,  0.5f,	(xb + 1) * scale, yb * scale,		// Bottom right
//		-0.5f, -0.5f,  0.5f,	xb * scale, yb * scale,			// Bottom left
//		-0.5f, -0.5f, -0.5f,	xb * scale, (yb + 1) * scale,		// Back Bottom left
//		 0.5f, -0.5f, -0.5f,	(xb + 1) * scale, (yb + 1) * scale,// Back Bottom right
//	};
//
//	unsigned int indices[3 * 12] = {
//		0, 1, 2,
//		0, 2, 3,
//
//		4, 5, 6,
//		4, 6, 7,
//
//		8, 9, 10,
//		8, 10, 11,
//
//		12, 13, 14,
//		12, 14, 15,
//
//		16, 17, 18,
//		16, 18, 19,
//
//		20, 21, 22,
//		20, 22, 23,
//	};
//
//	// Create Layout
//	std::vector<JoyGL::VertexArrayLayoutElement> layoutVector = {
//		{0, 3, GL_FLOAT, GL_FALSE, sizeof(float)},
//		{1, 2, GL_FLOAT, GL_FALSE, sizeof(float)},
//	};
//	std::shared_ptr<JoyGL::VertexArrayLayout> layout = std::make_shared<JoyGL::VertexArrayLayout>(
//		layoutVector
//	);
//
//	// Create Buffers
//	std::shared_ptr<JoyGL::VertexBuffer> vb = std::make_shared<JoyGL::VertexBuffer>(vertices, sizeof(vertices));
//	std::shared_ptr<JoyGL::IndexBuffer> ib = std::make_shared<JoyGL::IndexBuffer>(indices, sizeof(indices));
//	JoyGL::VertexArray va(vb, ib, layout);
//
//	JoyGL::PerspectiveCamera camera;
//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);
//	glm::mat4 model = glm::mat4(1.0f);
//	glm::mat4 view = glm::mat4(1.0f);
//	view = glm::translate(view, glm::vec3(-2.0f, 2.0f, -7.0f));
//	camera.SetProjection(projection);
//	camera.SetView(view);
//
//	glEnable(GL_DEPTH_TEST);
//
//	while (!glfwWindowShouldClose(window.GetNativeWindowPointer())) {
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
//		
//		blockShader->Bind();
//		texture.Bind();
//
//		blockShader->UploadUniformMatrix4fv("u_View", glm::value_ptr(camera.GetView()));
//		blockShader->UploadUniformMatrix4fv("u_Projection", glm::value_ptr(camera.GetProjection()));
//		blockShader->UploadUniformMatrix4fv("u_Model", glm::value_ptr(model));
//
//		va.Bind();
//		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//		//JoyGL::Renderer.Render(VertexArray, Shader, Camera);
//
//		glfwSwapBuffers(window.GetNativeWindowPointer());
//		glfwPollEvents();
//
//	}
//}

/*
* Author: Isaac Muscat.
* Date: January 1, 2022.
* Description: Minecraft recreation in OpenGL.
*/

#include "pch.h"
#include "Game.h"

int main(void)
{
	Game game(1800, 1000, "Minecraft");
	game.Run();
	return 0;
}