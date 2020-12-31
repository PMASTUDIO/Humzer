#include "humpch.h"
#include "Scene.h"
#include "Components.h"
#include "../Renderer/Renderer.h"
#include "Entity.h"

namespace Humzer {

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		auto& tag = entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();

		tag.Tag = name.empty() ? "Empty Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep dt)
	{
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<PrimitiveRendererComponent>);
			for (auto entity : group) {
				auto& [transform, primitive] = group.get<TransformComponent, PrimitiveRendererComponent>(entity);

				switch (primitive.Shape)
				{
				case PrimitiveShape::CUBE:
					Renderer3D::DrawCube(transform, primitive.Color);
					break;
				case PrimitiveShape::QUAD:
					Renderer3D::DrawPlane(transform, primitive.Color);
					break;
				default:
					break;
				}

			}
		}

		{
			auto group = m_Registry.group<MeshRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto& [transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);

				if (mesh.Mesh) {
					Renderer3D::DrawMesh(transform, mesh.Mesh);
				}

			}
		}
	}

}
