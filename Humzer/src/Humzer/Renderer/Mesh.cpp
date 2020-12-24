#include "humpch.h"
#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "RenderCommand.h"

namespace Humzer {

	static const uint32_t s_MeshImportFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals;

	Mesh::Mesh(const std::string& filename) : m_FilePath(filename)
	{
		m_Importer = CreateScope<Assimp::Importer>();
		const aiScene* scene = m_Importer->ReadFile(filename, s_MeshImportFlags);

		if (!scene || !scene->HasMeshes())
			HUM_CORE_ERROR("Failed to load mesh file: {0}", filename);

		m_MeshShader = Shader::Create("Resources/shaders/mesh_base_shader.vs", "Resources/shaders/mesh_base_shader.fs");
		
		processNode(scene->mRootNode, scene);
		
		// MESH SETUP
		m_VertexArray = VertexArray::Create(); // Create VAO
		
		auto vb = VertexBuffer::Create(m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex)); // Add vertex data to VBO
		vb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(vb);

		auto ib = IndexBuffer::Create(m_Indices.data(), m_Indices.size() * sizeof(Index));
		m_VertexArray->SetIndexBuffer(ib); 
		m_Scene = scene;
		HUM_CORE_INFO("Loaded {0} meshes, with {1} vertices and {2} indices, from {3}, successfuly!", scene->mNumMeshes, m_StaticVertices.size(), m_Indices.size(), m_FilePath);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Render(Timestep ts, PerspectiveCamera& camera, const glm::mat4& transform /*= glm::mat4(1.0f)*/)
	{
		// SHADER SET UP
		m_MeshShader->Bind();

		m_MeshShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		glm::mat4 transformTest = glm::translate(glm::mat4(1.0), { 0.0, 0.0, 0.0 }) * glm::scale(glm::mat4(1.0), { 1.0, 1.0, 1.0 });
		m_MeshShader->SetMat4("u_Transform", transformTest);

		// DRAWING
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
	}

	void Mesh::processNode(aiNode* node, const aiScene* scene)
	{
		// PROCESS MESHES

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		// PROCESS NODE MESHES
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			m_Submeshes.push_back(processMesh(mesh, scene, vertexCount, indexCount));
		}

		// PROCESS CHILDREN'S MESHES
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Humzer::Submesh Mesh::processMesh(aiMesh* mesh, const aiScene* scene, uint32_t vertexCount, uint32_t indexCount)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Texture2D> texture;

		Submesh submesh;
		submesh.BaseVertex = vertexCount;
		submesh.BaseIndex = indexCount;
		submesh.MaterialIndex = mesh->mMaterialIndex;
		submesh.IndexCount = mesh->mNumFaces * 3;

		// PROCESS VERTICES
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->HasTextureCoords(0))
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			m_StaticVertices.push_back(vertex);
		}
		
		// PROCESS INDICES
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
				HUM_CORE_ERROR("Error loading model {0}, models needs to have exactly 3 indices!", m_FilePath);

			m_Indices.push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
		}

		return submesh;
	}

}

