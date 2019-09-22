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
	class TestTexture2D : public Test {
	private:
		glm::vec3 m_Translation;
		
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr <VertexBuffer> m_VB;
		std::unique_ptr <IndexBuffer> m_IB;
		std::unique_ptr <Shader> m_Shader;
		std::unique_ptr <Texture> m_Texture1;
		std::unique_ptr <Texture> m_Texture2;

		glm::mat4 m_Proj, m_View;
	public:
		TestTexture2D();
		~TestTexture2D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}