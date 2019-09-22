#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input {
public:
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {};
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {};
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {};
};