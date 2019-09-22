#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace model {

	class Model {
	public:

		Model(const char* path) {
			loadModel(path);
		}

		void Draw(Shader& shader);

	private:

		std::vector<Mesh> meshes;
		std::string dir;

		std::vector<Texture> textures_loaded;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}
