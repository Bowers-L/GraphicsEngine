#pragma once

#include "Primitive.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace primitive {
	class Cube  {
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<IndexBuffer>  m_IBO;

		void setupBuffers();
	public:
		glm::vec3 m_Position;
		glm::vec3 m_Dimensions;

		Cube(glm::vec3 position, glm::vec3 dimensions);
		~Cube();

		void draw(Shader& shader);

	};
}
