#include "TestClearColor.h"

#include "Renderer.h"

#include "imgui/imgui.h"

namespace test {
	TestClearColor::TestClearColor()
		: m_ClearColor{ 0.0f, 0.0f, 1.0f, 1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::onUpdate(float deltaTime)
	{
	}

	void TestClearColor::onRender()
	{
		Renderer renderer;
		renderer.setClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		renderer.clear();
	}

	void TestClearColor::onImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}
