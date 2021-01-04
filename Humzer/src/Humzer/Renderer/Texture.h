#pragma once

#include <string>
#include "Humzer/Core/Core.h"

namespace Humzer {

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class HUMZER_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

		virtual bool Loaded() const = 0;
	};

	class HUMZER_API TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(const std::vector<std::string> faces);
		static Ref<TextureCube> Create(const std::string& filepath);
	};

}
