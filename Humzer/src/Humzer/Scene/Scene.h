#pragma once

#include <entt.hpp>
#include "../Core/Timestep.h"

namespace Humzer {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep dt);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}