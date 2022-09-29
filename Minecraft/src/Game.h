#pragma once

#include <string>
#include <chrono>

#include <GLFW/glfw3.h>

#include "Player.h"
#include "World.h"
#include "Cursor.h"

class Game {
public:
	Game(int width, int height, const std::string& title);
	~Game();

	void Run();
	void CursorPosCallbackFunction(double xPos, double yPos);

private:
	void Update();
	void Draw() const;

private:
	int m_Width;
	int m_Height;
	std::string m_Title;
	GLFWwindow* m_Window;

	std::unique_ptr<Cursor> m_Cursor;
	std::unique_ptr<Player> m_Player;
	std::unique_ptr<World> m_World;


};