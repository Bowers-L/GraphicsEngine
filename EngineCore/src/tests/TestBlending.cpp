#include "TestBlending.h"

#include "util/GLDebug.h"

#include "imgui/imgui.h"

#include <math.h>

#define WIDTH 640
#define HEIGHT 480

namespace test {
	TestBlending::TestBlending()
		: m_Translation(0, 0, 0),
		m_Proj(glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_T(0)

	{
		//rendering data

		float vertices[] = {
			0.0f, 0.0f, 0.0f, 1.0f,	//top left (0)
			0.0f, (float)HEIGHT / 2, 0.0f, 0.0f,	//bottom left (1)
			(float)WIDTH / 2, 0.0f, 1.0f, 1.0f,	//top right (2)
			(float)WIDTH / 2, (float)HEIGHT / 2, 1.0f, 0.0f,	//bottom right (3)
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 1
		};

		//Creating a vertex array on the stack

		//Creating a vertex buffer and passing it to the vertex array with a layout.
		m_VA = std::make_unique<VertexArray>();
		m_VB = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		m_VA->addBuffer(*m_VB, layout);

		//Creating an index buffer and passing it the indices
		m_IB = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);

		//bind texture
		m_Texture = std::make_unique<Texture>("texture_diffuse", "res/textures/CoverPage(retarded).PNG");
		m_Texture->bind(0);

		//set up blending
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		//Creating a shader and setting its uniforms.
		m_BasicShader = std::make_unique<Shader>("res/shaders/Basic.shader");

		m_TextureShader = std::make_unique<Shader>("res/shaders/Texture.shader");
		m_TextureShader->bind();
		m_TextureShader->setUniform1i("u_Texture", 0);	//Setting the texture to use to be at slot 0

		//unbinding
		m_VA->unbind();
		m_VB->unbind();
		m_IB->unbind();
		m_TextureShader->unbind();
	}

	TestBlending::~TestBlending()
	{
	}

	void TestBlending::onUpdate(float deltaTime)
	{
		m_T += deltaTime;
	}

	void TestBlending::onRender()
	{
		Renderer renderer;

		renderer.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer.clear();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_TextureShader->bind();
			m_TextureShader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*m_VA, *m_IB, *m_TextureShader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_BasicShader->bind();
			m_BasicShader->setUniformMat4f("u_MVP", mvp);
			m_BasicShader->setUniform4f("u_Color", 0.5 + 0.5 * sin(m_T), 0.5 + 0.5 * cos(m_T), 1.0f, 0.5f);
			renderer.draw(*m_VA, *m_IB, *m_BasicShader);
		}
	}

	void TestBlending::onImGuiRender()
	{
		ImGui::SliderFloat("Translation x", &m_Translation.x, 0.0f, (float)WIDTH / 2);
		ImGui::SliderFloat("Translation y", &m_Translation.y, 0.0f, (float)HEIGHT / 2);
	}
}
