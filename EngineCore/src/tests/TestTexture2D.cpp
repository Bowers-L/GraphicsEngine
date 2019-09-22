#include "TestTexture2D.h"

#include "util/GLDebug.h"

#include "imgui/imgui.h"

#define WIDTH 640
#define HEIGHT 480

namespace test {
	TestTexture2D::TestTexture2D()
		: m_Translation(0, 0, 0), 
		m_Proj(glm::ortho(0.0f, (float) WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))

	{
		//rendering data

		float vertices[] = {
			0.0f, 0.0f, 0.0f, 0.0f,	//top left (0)
			0.0f, (float) HEIGHT / 2, 0.0f, 1.0f,	//bottom left (1)
			(float) WIDTH / 2, 0.0f, 1.0f, 0.0f,	//top right (2)
			(float) WIDTH / 2, (float) HEIGHT / 2, 1.0f, 1.0f,	//bottom right (3)
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
		m_Texture1 = std::make_unique<Texture>("texture_diffuse", "res/textures/CoverPage(retarded).PNG");
		m_Texture1->bind(0);

		//set up blending
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		//Creating a shader and setting its uniforms.
		m_Shader = std::make_unique<Shader>("res/shaders/Texture.shader");
		m_Shader->bind();
		m_Shader->setUniform1i("u_Texture", 0);	//Setting the texture to use to be at slot 0

		//unbinding
		m_VA->unbind();
		m_VB->unbind();
		m_IB->unbind();
		m_Shader->unbind();
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::onUpdate(float deltaTime)
	{
	}

	void TestTexture2D::onRender()
	{
		Renderer renderer;

		renderer.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer.clear();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;

		m_Shader->bind();
		m_Shader->setUniformMat4f("u_MVP", mvp);
		m_Shader->setUniform1i("u_Texture", 0);

		renderer.draw(*m_VA, *m_IB, *m_Shader);
	}

	void TestTexture2D::onImGuiRender()
	{
		ImGui::SliderFloat("Translation x", &m_Translation.x, 0.0f, (float)WIDTH / 2);
		ImGui::SliderFloat("Translation y", &m_Translation.y, 0.0f, (float)HEIGHT / 2);
	}
}
