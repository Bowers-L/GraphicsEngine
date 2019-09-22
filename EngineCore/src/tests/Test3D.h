#pragma once

#include "Test.h"

#include "buffers/Buffers.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp" 
#include "glm/ext.hpp"

#include "_3DCamera.h"

#include <memory>

namespace test {

	class Test3D : public Test {
	private:



		std::unique_ptr<VertexBuffer> CubeVB;
		std::unique_ptr<IndexBuffer> CubeIB;

		std::unique_ptr<VertexArray> LightVA;
		std::unique_ptr<VertexArray> ObjVA;

		std::unique_ptr<Shader> LightShader;
		std::unique_ptr<Shader> ObjShader;

		std::unique_ptr<Texture> ObjDiffuseMap;
		std::unique_ptr<Texture> ObjSpecularMap;
		std::unique_ptr<Texture> ObjEmissionMap;
		

		glm::vec3 PointLightPositions[4];

		glm::mat4 Proj, View, Model;

		_3DCamera Cam;

		float time, radius;

		float *vertices, *vertices1;

		int fps;

		//lighting
	public:
		Test3D();
		~Test3D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

		void processInput(GLFWwindow* window, float deltaTime) override;
	};
}
