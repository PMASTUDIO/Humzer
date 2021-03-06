#pragma once

#include <glm/glm.hpp>
#include <string>
#include "../Core/Timestep.h"
#include "../Core/Core.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include <Humzer\Renderer\Camera.h>

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace Assimp {
	class Importer;
}


namespace Humzer {
	
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	struct Index
	{
		uint32_t V1, V2, V3;
	};

	class Submesh {
	public:
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;

		glm::mat4 Transform;
	};

	class HUMZER_API Mesh
	{
	public:
		Mesh(std::string filename);
		Mesh(std::string name, std::string filename);
		~Mesh();

		const std::string& GetName();

		//void Render(Timestep ts, Camera& camera, const glm::mat4& transform = glm::mat4(1.0f));

		/*inline Ref<Shader> GetMeshShader() { return m_MeshShader; }*/
		inline const std::string& GetFilePath() const { return m_FilePath; }

		inline std::vector<Ref<Texture2D>>& GetTexturesList() { return m_Textures; }
	private:
		void processMaterials(const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
		Submesh processMesh(aiMesh* mesh, const aiScene* scene, uint32_t vertexCount, uint32_t indexCount);
	private:
		std::string m_FilePath;
		std::string m_Name;

		/*Ref<Shader> m_MeshShader;*/

		Scope<Assimp::Importer> m_Importer;

		const aiScene* m_Scene;

		std::vector<Ref<Texture2D>> m_Textures;

		// GEOMETRY
		Ref<VertexArray> m_VertexArray;
		std::vector<Vertex> m_StaticVertices;
		std::vector<Index> m_Indices;

		std::vector<Submesh> m_Submeshes;

		friend class Renderer3D;
	};

	class MeshLibrary {
	public:
		static void Add(const Ref<Mesh>& mesh);
		static Ref<Mesh> Load(const std::string& name, const std::string& filepath);

		static Ref<Mesh> Get(const std::string& name);
	private:
		static std::unordered_map<std::string, Ref<Mesh>> s_Meshes;
	};
}
