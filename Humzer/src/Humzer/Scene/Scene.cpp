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
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		glm::vec3* cameraPos; // #TEMP (used to draw mesh with reflections)
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					cameraPos = &transform.Translation;
					break;
				}
			}
		}

		HUM_ASSERT(mainCamera, "Scene does not contain any cameras!");

		// 2D RENDERER
		
#if HUM_DEBUG
		Renderer2D::ResetStats();
#endif

		Renderer2D::BeginScene(*mainCamera, *cameraTransform);
		{
			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if(sprite.Texture)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.TilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
		}
		Renderer2D::EndScene();
		

		// 3D RENDERER
		Renderer3D::BeginScene(*mainCamera, *cameraTransform, *cameraPos);
		{
			if (m_SkyboxTexture)
				Renderer3D::DrawSkybox(m_SkyboxTexture);

			{
				auto group = m_Registry.group<PrimitiveRendererComponent>(entt::get<TransformComponent>);
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
		}
		Renderer3D::EndScene();
		
	}

}
