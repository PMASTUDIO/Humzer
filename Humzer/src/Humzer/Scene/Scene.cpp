#include "humpch.h"
#include "Scene.h"
#include "Components.h"
#include "../Renderer/Renderer.h"
#include "Entity.h"
#include "../Renderer/Texture.h"
#include "glad/glad.h"

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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::SetSkybox(const Ref<TextureCube>& skybox)
	{
		m_SkyboxTexture = skybox;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cc = view.get<CameraComponent>(entity);
			cc.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnUpdateRuntime(Timestep dt)
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

		// HUM_ASSERT(mainCamera, "Scene does not contain any cameras!");
		if (!mainCamera)
			return;

		// 2D RENDERER
		
#if HUM_DEBUG
		Renderer2D::ResetStats();
#endif

		Renderer2D::BeginScene(*mainCamera, *cameraTransform);
		{
			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if (sprite.Texture)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, (uint32_t)entity, sprite.TilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)entity);
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
						Renderer3D::DrawCube(transform.GetTransform(), primitive.Color, (uint32_t)entity);
						break;
					case PrimitiveShape::QUAD:
						Renderer3D::DrawPlane(transform.GetTransform(), primitive.Color, (uint32_t)entity);
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
						Renderer3D::DrawMesh(transform.GetTransform(), mesh.Mesh, (uint32_t)entity);
					}

				}
			}
		}
		Renderer3D::EndScene();
		
	}

	void Scene::OnUpdateEditor(Timestep dt, EditorCamera& editorCamera)
	{
#if HUM_DEBUG
		Renderer2D::ResetStats();
#endif
		{
			Renderer2D::BeginScene(editorCamera);
		
			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if (sprite.Texture)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, (uint32_t)entity, sprite.TilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)entity);
			}
		
			Renderer2D::EndScene();
		}


		// 3D RENDERER
		{
			Renderer3D::BeginScene(editorCamera);
		
			if (m_SkyboxTexture)
				Renderer3D::DrawSkybox(m_SkyboxTexture);

			{
				auto group = m_Registry.group<PrimitiveRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto& [transform, primitive] = group.get<TransformComponent, PrimitiveRendererComponent>(entity);

					switch (primitive.Shape)
					{
					case PrimitiveShape::CUBE:
						Renderer3D::DrawCube(transform.GetTransform(), primitive.Color, (uint32_t)entity);
						break;
					case PrimitiveShape::QUAD:
						Renderer3D::DrawPlane(transform.GetTransform(), primitive.Color, (uint32_t)entity);
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
						Renderer3D::DrawMesh(transform.GetTransform(), mesh.Mesh, (uint32_t)entity);
					}

				}
			}
		
			Renderer3D::EndScene();
		}
	}

	Humzer::Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& cc = view.get<CameraComponent>(entity);
			if (cc.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<MeshRendererComponent>(Entity entity, MeshRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<PrimitiveRendererComponent>(Entity entity, PrimitiveRendererComponent& component)
	{
	}

}
