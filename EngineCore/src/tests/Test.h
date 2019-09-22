#pragma once

#include "Input.h"

namespace test {
	class Test : public Input {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}
		virtual void processInput(GLFWwindow* window, float deltaTime) {}
	};
}