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

		void SetSkybox(const Ref<TextureCube>& skybox);

		void OnUpdate(Timestep dt);
	private:
		entt::registry m_Registry;

		Ref<TextureCube> m_SkyboxTexture;

		friend class Entity;
		friend class SceneSerializer;
	};

}