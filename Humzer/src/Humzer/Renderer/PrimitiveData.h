#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Humzer {

	class Primitive {
	public:
		static void getTriData(std::vector<float>& vertices, std::vector<uint32_t>& indices);
		static void getQuadData(std::vector<float>& vertices, std::vector<uint32_t>& indices);
		static void getCubeData(std::vector<float>& vertices, std::vector<uint32_t>& indices);
		static void getSkyboxData(std::vector<float>& vertices);
	};

}