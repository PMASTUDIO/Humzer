#pragma once

#include <entt.hpp>
#include "Scene.h"

namespace Humzer {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			if (HasComponent<T>()) {
				HUM_CORE_ERROR("Cannot add component: Entity already has component!");
			}
			HUM_ASSERT(!HasComponent<T>(), "Cannot add component: Entity already has component!");

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}
		
		template<typename T>
		T& GetComponent() {
			if (!HasComponent<T>()) {
				HUM_CORE_ERROR("Cannot get component: Entity does not has component!");
			}
			HUM_ASSERT(HasComponent<T>(), "Cannot get component: Entity does not has component!");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent() {
			if (!HasComponent<T>()) {
				HUM_CORE_ERROR("Cannot remove component: Entity does not has component!");
			}
			HUM_ASSERT(HasComponent<T>(), "Cannot remove component: Entity does not has component!");

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}
