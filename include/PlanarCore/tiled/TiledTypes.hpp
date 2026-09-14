#pragma once

#include <filesystem>
#include "PropertyBag.hpp"
#include "runtime/Tag.hpp"

namespace core::tilemap {
	enum class ImageType {
		TILESHEET,
		OBJECT,

		NONE,
	};

	enum class ObjectType {
		SPRITE,
		RECT,
		POINT,

		NONE,
	};

	struct TileImageInfo {
		int firstGid = -1;
		int tileWidth = -1;
		int tileHeight = -1;
		int imageWidth = -1;
		int imageHeight = -1;
		int id = -1;

		std::filesystem::path path;
		std::filesystem::path convertedPath;
		bool converted = false;
		ImageType type = ImageType::NONE;
	};

	struct GIDInfo {
		int gid = 0;
		bool vFlip = false;
		bool hFlip = false;
	};

	struct Tile {
		Vec2<int16_t> pos;
		uint16_t texIdx{};
		Vec2<uint16_t> size;

		bool flipX = false;
		bool flipY = false;
	};

	class TileObject {
	public:
		PropertyBag properties{};

		std::string name;
		Vec2<uint16_t> size;
		Vec2<int16_t> pos;

		ObjectType objectType = ObjectType::NONE;
		runtime::Tag tag;

		ObjectId id{};

		int16_t texIdx = -1;

		// GID attributes
		bool flipX = false;
		bool flipY = false;

		bool dontDraw = false;
	};
}
