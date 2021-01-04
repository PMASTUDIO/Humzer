#include "humpch.h"
#include "Scene.h"
#include "Components.h"
#include "../Renderer/Renderer.h"
#include "Entity.h"
#include "../Renderer/Texture.h"

namespace Humzer {

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	void Scene::Init()
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

	void Scene::SetSkybox(const Ref<TextureCube>& skybox)
	{
		m_SkyboxTexture = skybox;
	}

	void Scene::OnUpdate(Timestep dt)
	{
		PerspectiveCamera* camera = nullptr;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view) {
				auto& comp = view.get<CameraComponent>(entity);

				if (comp.Primary) {
					camera = &comp.Camera;
				}

				break;
			}
		}

		HUM_ASSERT(camera, "Scene does not contain any cameras!");
		camera->OnUpdate(dt);

		Renderer3D::BeginScene(*camera);

		if(m_SkyboxTexture)
			Renderer3D::DrawSkybox(m_SkyboxTexture);

		{
			auto group = m_Registry.group<TransformComponent>(entt::get<PrimitiveRendererComponent>);
			for (auto entity : group) {
				auto& [transform, primitive] = group.get<TransformComponent, PrimitiveRendererComponent>(entity);

				switch (primitive.Shape)
				{
				case PrimitiveShape::CUBE:
					Renderer3D::DrawCube(transform.GetTransform(), primitive.Color);
					break;
				case PrimitiveShape::QUAD:
					Renderer3D::DrawPlane(transform.GetTransform(), primitive.Color);
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
					Renderer3D::DrawMesh(transform.GetTransform(), mesh.Mesh);
				}

			}
		}

		Renderer3D::EndScene();
	}

}
