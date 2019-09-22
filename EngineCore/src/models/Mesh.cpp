#include "Mesh.h"
#include "Renderer.h"

namespace model {
	model::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void model::Mesh::setupMesh()
	{
		VAO = std::make_unique<VertexArray>();

		VBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

		IBO = std::make_unique<IndexBuffer>(&indices[0], indices.size(), GL_STATIC_DRAW);

		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(3);
		layout.push<float>(2);

		VAO->addBuffer(*VBO, layout);
	}

	void model::Mesh::draw(Shader& shader)
	{
		unsigned int numDiffuse = 0;
		unsigned int numSpecular = 0;
		for (unsigned int i = 0; i < textures.size(); i++) {

			std::string number;
			std::string type = textures[i].type;
			if (type == "texture_diffuse") {
				number = std::to_string(numDiffuse++);
			}
			else if (type == "texture_specular") {
				number = std::to_string(numSpecular++);
			}

			shader.setUniform1i("u_Material." + type + number, i);
			shader.setUniform1f("u_Material.shininess", 8);

			textures[i].bind(i);
		}

		Renderer::draw(*VAO, *IBO, shader);
	}

	void model::Mesh::printMeshProperties() {
		for (int i = 0; i < 10; i++) {
			PRINTLN(VEC3PRINTFORMAT(vertices[i].Position) << " " << VEC3PRINTFORMAT(vertices[i].Normal) << " " << VEC2PRINTFORMAT(vertices[i].TexCoords));
		}

		for (int i = 0; i < 10; i++) {
			PRINTLN(indices[i]);
		}

		for (int i = 0; i < textures.size(); i++) {
			PRINTLN(textures[i].type);
		}
	}
}
