#pragma once

#include "Test.h"

#include "Shader.h"
#include "Renderer.h"

#include "models/Model.h"
#include "primitives/Cube.h"

#include "_3DCamera.h"

#include "glm/glm.hpp" 
#include "glm/ext.hpp"

#include <memory>

using namespace model;
using namespace primitive;

namespace test {
	class TestModel : public Test {
	private:
		Model model;
		Cube light0;
		Cube light1;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> LightShader;
		_3DCamera camera;

		glm::mat4 mModel, mView, mProj;

		glm::vec3 lightPos0, lightPos1;

		int fps;
	public:
		TestModel();
		~TestModel();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

		void processInput(GLFWwindow* window, float deltaTime) override;
	};
}