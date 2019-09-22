#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define WIDTH 640
#define HEIGHT 480

void Renderer::clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
	GLCall(glClearColor(r, g, b, a));
}

void Renderer::draw(const VertexArray& va, const Shader& shader) {
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	va.bind();
	ib.bind();
	shader.bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

	va.unbind();
	ib.unbind();
	shader.unbind();
}

/*
void Renderer::drawLine(glm::vec3 p1, glm::vec3 p2, float weight, const glm::vec4& color) {
	glm::vec3 normDir = glm::normalize(glm::vec3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z));

	float positions[] = { p1.x + normDir.y * weight / 2, p1.y - normDir.x * weight / 2, p1.z,
						p1.x - normDir.y * weight / 2, p1.y + normDir.x * weight / 2, p1.z,
						p2.x + normDir.y * weight / 2, p2.y - normDir.x * weight / 2, p2.z,
						p2.x - normDir.y * weight / 2, p2.y + normDir.x * weight / 2, p2.z,
	};

	unsigned int indices[] = { 0, 1, 2,
								2, 3, 0,
	};

	VertexArray va;

	VertexBuffer vb(positions, 3 * 4 * sizeof(float), GL_STATIC_DRAW);
	VertexBufferLayout layout;
	layout.push<float>(3);
	va.addBuffer(vb, layout);

	IndexBuffer ib(indices, 6, GL_STATIC_DRAW);

	glm::mat4 mvp = glm::ortho(0, WIDTH, HEIGHT, 0, 1, -1);

	Shader shader("res/shaders/Basic.shader");
	shader.bind();
	shader.setUniformMat4f("u_MVP", mvp);
	shader.setUniform4f("u_Color", color.r, color.g, color.b, color.a);

	draw(va, ib, shader);
}
*/
