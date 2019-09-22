#include "Cube.h"

primitive::Cube::Cube(glm::vec3 position, glm::vec3 dimensions)
{
	m_Position = position;
	m_Dimensions = dimensions;

	setupBuffers();
}

primitive::Cube::~Cube()
{
}

void primitive::Cube::setupBuffers()
{
	//vertex data
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,	//bottom left far 0
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,	//bottom right far 1
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f,	//top right far 2
	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f,	//top left far 3

	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left near 4
	 0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	//bottom right near 5
	 0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	//top right near 6
	-0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,	//top left near 7

	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	//bottom left far 8 *
	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,	//bottom left near 9
	-0.5f,  0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top left near 10
	-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,	//top left far 11 *

	 0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	//bottom right near 12
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,	//bottom right far 13 *
	 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right far 14 *
	 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,	//top right near 15

	-0.5f,  0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	//top left near 16
	 0.5f,  0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//top right near 17
	 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,	//top right far 18 *
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	//top left far 19 *

	-0.5f,  -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,	//bottom left near 20
	 0.5f,  -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f,	//bottom right near 21
	 0.5f,  -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f,	//bottom right far 22 *
	-0.5f,  -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,	//bottom left far 23 *
	};

	//Index data

	unsigned int indices[36];
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			switch (j) {
			case 0:
			case 1:
			case 2:
				indices[i * 6 + j] = i * 4 + j;
				break;
			case 3:
			case 4:
				indices[i * 6 + j] = i * 4 + j - 1;
				break;
			case 5:
				indices[i * 6 + j] = i * 4;
				break;
			}
		}
	}

	//Vertex Array

	m_VAO = std::make_unique<VertexArray>();

	//Vertex Buffer + layout

	m_VBO = std::make_unique<VertexBuffer>(vertices, 24 * 8 * sizeof(float), GL_STATIC_DRAW);
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);

	m_VAO->addBuffer(*m_VBO, layout);

	//Index Buffer
	m_IBO = std::make_unique<IndexBuffer>(indices, 36, GL_STATIC_DRAW);
}

void primitive::Cube::draw(Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, m_Position);
	model = glm::scale(model, m_Dimensions);
	
	PRINTLN(m_Position.y);
	shader.setUniformMat4f("u_Model", model);

	Renderer::draw(*m_VAO, *m_IBO, shader);
}
