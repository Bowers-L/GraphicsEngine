#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <Shader.h>
#include <buffers/Buffers.h>

#include <glad/glad.h>
#include <util/GLDebug.h>

#include <Texture.h>

namespace model {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;	//Textures can be diffuse or specular maps

		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
		void draw(Shader& shader);

		//for debugging only
		void printMeshProperties();

	private:
		std::unique_ptr<VertexArray> VAO;
		std::unique_ptr<VertexBuffer> VBO;
		std::unique_ptr<IndexBuffer> IBO;

		void setupMesh();
	};
}
