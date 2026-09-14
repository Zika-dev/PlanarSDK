#pragma once

#include <string>

#include "Core.hpp"
#include "TiledTypes.hpp"
#include "graphics/library/TextureHandle.hpp"

namespace core::graphics {
	class IGraphicsAPI;
}

namespace core::tilemap {

	class PLANAR_API ObjectLayer {
	public:

		void draw() const;

		TileObject* getObject(std::string name) {
			if (objectNameQueryMap.count(name) > 0) {
				return &objects[objectNameQueryMap[name]];
			}

			return nullptr;
		}

		TileObject* getObject(uint16_t id) {
			if (objectIdQueryMap.count(id) > 0) {
				return &objects[objectIdQueryMap[id]];
			}

			return nullptr;
		}

		void addObject(TileObject object) {
			objects.emplace_back(object);
			objectNameQueryMap[object.name] = objects.size() - 1;
			objectIdQueryMap[object.id] = objects.size() - 1;
		}

		uint16_t id;
		std::string name;
		std::vector<TileObject> objects;
		std::vector<graphics::TextureHandle> textures;
		int16_t order = 0;

	private:
		std::unordered_map<std::string, uint16_t> objectNameQueryMap;
		std::unordered_map<uint16_t, uint16_t> objectIdQueryMap;
	};
} 
