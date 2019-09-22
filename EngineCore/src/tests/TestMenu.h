#pragma once

#include "Test.h"

#include <vector>
#include <string>
#include <functional>

#include "util/Macros.h"

namespace test {
	class TestMenu : public Test {
	private:
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
		Test*& m_CurrentTest;
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void onRender() override;
		void onImGuiRender() override;

		template<typename T> 
		void addTest(const std::string& name) {
			PRINTLN("Adding Test");
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};
}