#pragma once

#include <vector>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <unordered_map>
#include <string>

#include "Core.hpp"
#include "ObjectLayer.hpp"
#include "TileLayer.hpp"
#include "TiledTypes.hpp"

namespace core::tilemap {

	struct PLANAR_API Tilemap {
		std::vector<TileLayer> tileLayers;
		std::vector<ObjectLayer> objectLayers;

		std::unordered_map<std::string, uint16_t> objectLayerNameQueryMap; // Name : index in vector
		std::unordered_map<uint16_t, uint16_t> objectLayerIdQueryMap; // ID : index in vector

		std::unordered_map<std::string, uint16_t> tileLayerNameQueryMap; // Name : index in vector
		std::unordered_map<uint16_t, uint16_t> tileLayerIdQueryMap; // ID : index in vector

		ObjectLayer* getObjectLayer(std::string name) {
			return objectLayerNameQueryMap.count(name) > 0 ? &objectLayers[objectLayerNameQueryMap[name]] : nullptr;
		}

		ObjectLayer* getObjectLayer(uint16_t id) {
			return objectLayerIdQueryMap.count(id) > 0 ? &objectLayers[objectLayerIdQueryMap[id]] : nullptr;
		}

		TileLayer* getTileLayer(std::string name) {
			return tileLayerNameQueryMap.count(name) > 0 ? &tileLayers[tileLayerNameQueryMap[name]] : nullptr;
		}

		TileLayer* getTileLayer(uint16_t id) {
			return tileLayerIdQueryMap.count(id) > 0 ? &tileLayers[tileLayerIdQueryMap[id]] : nullptr;
		}
	};

	PLANAR_API Tilemap decodeTiledMap(std::filesystem::path path);

	GIDInfo getGidInfo(uint32_t gid);

	// Gets information about all of the tilesets that the tilemap uses
	std::vector<TileImageInfo> getTilemapImageInfo(std::filesystem::path path, const nlohmann::json& tilesetData, std::filesystem::path convertedDir);

	bool convertTileImage(const TileImageInfo& image, const std::filesystem::path& convertedDir, std::string& outDir);

	void parseTileLayer(std::vector<TileLayer>& tileLayers, const nlohmann::json& layerData, std::unordered_map<uint16_t, graphics::TextureHandle>& loadedHandles, Vec2<uint16_t> tileSize);

	void parseObjectLayer(std::vector<ObjectLayer>& objectLayers, const nlohmann::json& layerData, std::unordered_map<uint16_t, graphics::TextureHandle>& loadedHandles);

	void parseProperty(const nlohmann::json& property, TileObject& object);
}
