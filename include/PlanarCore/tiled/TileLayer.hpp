#pragma once

#include <vector>
#include <cstdint>

#include "Core.hpp"
#include "Utils.hpp"
#include "graphics/library/TextureHandle.hpp"
#include "TiledTypes.hpp"

namespace core::graphics {
	class IGraphicsAPI;
}

namespace core::tilemap {
	class PLANAR_API TileLayer {

	public:

		void draw() const;

		uint16_t id;
		std::string name;

		std::vector<graphics::TextureHandle> textures;
		std::vector<Tile> tiles;

		Vec2<uint16_t> tileSize;

		int16_t order = 0;
	};
}
