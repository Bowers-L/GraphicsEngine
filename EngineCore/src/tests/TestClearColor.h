#pragma once

#include "Test.h"

namespace test {
	class TestClearColor : public Test {
	private:
		float m_ClearColor[4];
	public:
		TestClearColor();
		~TestClearColor();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}