#include "humpch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"

// YAML DECODERS
namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Humzer {

	// OVERLOADS
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	// EO OVERLOADS

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene)
	{
		
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // ENTITY MAP
		out << YAML::Key << "Entity" << YAML::Value << "000000000000"; // #TODO ENTITY ID

		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent MAP

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;   // TagComponent MAP
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<PrimitiveRendererComponent>())
		{
			out << YAML::Key << "PrimitiveRendererComponent";
			out << YAML::BeginMap; // PrimitiveRendererComponent MAP

			auto& primitiveRendererComponent = entity.GetComponent<PrimitiveRendererComponent>();
			out << YAML::Key << "Shape" << YAML::Value << static_cast<int>(primitiveRendererComponent.Shape);
			out << YAML::Key << "Color" << YAML::Value << primitiveRendererComponent.Color;

			out << YAML::EndMap; // PrimitiveRendererComponent MAP
		}

		if (entity.HasComponent<MeshRendererComponent>()) {
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::BeginMap; // MeshRendererComponent MAP

			auto& meshRendererComponent = entity.GetComponent<MeshRendererComponent>();
			out << YAML::Key << "AssetPath" << YAML::Value << meshRendererComponent.Mesh->GetFilePath();
			const std::string assetName = meshRendererComponent.Mesh->GetName();
			out << YAML::Key << "AssetName" << YAML::Value << assetName;

			out << YAML::EndMap; // MeshRendererComponent MAP
		}

		out << YAML::EndMap;   // ENTITY MAP
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::Comment("This is a Humzer scene file stored under " + filepath);
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		
		if (m_Scene->m_SkyboxTexture) {
			out << YAML::Key << "Skybox" << YAML::Value << YAML::BeginSeq;

			std::vector<std::string> faces =  m_Scene->m_SkyboxTexture->GetFacesPath();
			for (auto face : faces) {
				out << face;
			}

			out << YAML::EndSeq;
		}
	
			out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// #TODO
		HUM_ASSERT(false, "Runtime serialization not implemented!");

	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		HUM_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		// Skybox
		auto skyboxFaces = data["Skybox"];
		if (skyboxFaces) {
			std::vector<std::string> skyboxPathFaces = {};
			for (auto face : skyboxFaces) {
				skyboxPathFaces.push_back(face.as<std::string>());
			}

			m_Scene->SetSkybox(TextureCube::Create(skyboxPathFaces));
		}

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {

				uint64_t uuid = entity["Entity"].as<uint64_t>(); // #TODO ENTITY UUID

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				HUM_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				// ENTITY CREATION
				Entity deserializedEntity = m_Scene->CreateEntity(name); // #TODO eventually we construct with ...->CreateEntity(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms, that's why we get component
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto primitiveRendererComponent = entity["PrimitiveRendererComponent"];
				if (primitiveRendererComponent)
				{
					// Entities always have transforms, that's why we get component
					auto& prc = deserializedEntity.AddComponent<PrimitiveRendererComponent>();
					prc.Shape = static_cast<PrimitiveShape>(primitiveRendererComponent["Shape"].as<int>());
					prc.Color = primitiveRendererComponent["Color"].as<glm::vec4>();
				}

				auto meshRendererComponent = entity["MeshRendererComponent"];
				if (meshRendererComponent)
				{
					// Entities always have transforms, that's why we get component
					auto& mrc = deserializedEntity.AddComponent<MeshRendererComponent>();
					mrc.Mesh = MeshLibrary::Load(meshRendererComponent["AssetName"].as<std::string>(), meshRendererComponent["AssetPath"].as<std::string>());
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// #TODO
		HUM_ASSERT(false, "Runtime deserialization not implemented!");

		return false;
	}
	
}
