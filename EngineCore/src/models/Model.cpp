#include "Model.h"

namespace model {

	void Model::loadModel(std::string path)
	{
		Assimp::Importer importer;

		//load scene
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		//check for errors
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			PRINTLN("Error Assimp: " << importer.GetErrorString() << std::endl);
			return;
		}

		dir = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{

		//process all meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			meshes.push_back(processMesh(mesh, scene));
		}

		//process all nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		//PRINTLN("processing mesh");
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		//vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			//position
			{
				glm::vec3 pos;
				pos.x = mesh->mVertices[i].x;
				pos.y = mesh->mVertices[i].y;
				pos.z = mesh->mVertices[i].z;

				vertex.Position = pos;
			}


			//normal
			{
				glm::vec3 normal;
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.Normal = normal;
			}

			//texture  coords
			if(mesh->mTextureCoords[0]) {
				glm::vec2 texCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = texCoords;
			}

			vertices.push_back(vertex);
		}

		//indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				
				indices.push_back(face.mIndices[j]);
			}
		}

		//materials
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());


			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}


		return Mesh(vertices, indices, textures);
	}

	//loads all textures of one type from a material
	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
			bool textureLoaded = false;

			aiString path;
			material->GetTexture(type, i, &path);

			//looping through already loaded textures
			for (unsigned int j = 0; j < textures_loaded.size(); j++) {

				if (std::strcmp(textures_loaded[j].path.data(), path.C_Str()) == 0) {
					textures.push_back(textures_loaded[j]);
					textureLoaded = true;
					break;
				}

			}

			if (!textureLoaded) {
				//need to load the texture

				Texture texture(typeName, std::string("res/models/nanosuit/").append(path.C_Str()) );

				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}

	void Model::Draw(Shader& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader);
		}
	}
}

