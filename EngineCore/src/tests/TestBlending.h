#pragma once

#include "Test.h"

#include "buffers/Buffers.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp" 
#include "glm/ext.hpp"

#include <memory>

namespace test {
	class TestBlending : public Test {
	private:
		glm::vec3 m_Translation;

		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr <VertexBuffer> m_VB;
		std::unique_ptr <IndexBuffer> m_IB;
		std::unique_ptr <Shader> m_BasicShader;
		std::unique_ptr <Shader> m_TextureShader;
		std::unique_ptr <Texture> m_Texture;

		glm::mat4 m_Proj, m_View;

		float m_T;
	public:
		TestBlending();
		~TestBlending();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}