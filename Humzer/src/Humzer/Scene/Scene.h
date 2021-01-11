#pragma once

#include <entt.hpp>
#include "../Core/Timestep.h"

namespace Humzer { class TextureCube; }

namespace Humzer {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		void Init();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void SetSkybox(const Ref<TextureCube>& skybox);

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdate(Timestep dt);

		Entity GetPrimaryCamera();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;

		Ref<TextureCube> m_SkyboxTexture;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPannel;
	};

}