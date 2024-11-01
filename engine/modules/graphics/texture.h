#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace muggle
{
	enum class TextureType
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DArray,
		Texture2DArray,
		TextureCubeArray,
		Count
	};

	struct TextureCreation
	{
		void* initialData {nullptr};
		uint32_t width {0};
		uint32_t height {0};
		uint32_t depth {1};
		uint32_t mipLevels {1};
        uint32_t flags {0};

	};
}