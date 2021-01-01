#pragma once

#include "Scene.h"
#include "../Core/Core.h"

namespace YAML { class Emitter; }

namespace Humzer {

	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);
	private:
		Ref<Scene> m_Scene;
	};

}
