#pragma once

#include <cstdint>
#include "core.hpp"
#include "Utils.hpp"
#include "spdlog/spdlog.h"
#include "graphics/library/Texture.hpp"
#include <filesystem>

#include <string>

namespace core::codec {
	class PLANAR_API ConsoleCodec {

		struct Pixel {
			uint8_t r, g, b, a;
		};

		struct TexHeader {
			uint16_t width, height, totalRegions;
			uint32_t totalPixels;

			size_t serializeTo(uint8_t* out) const noexcept {
				size_t index = 0;

				writeInt16(out, width, index);
				writeInt16(out, height, index);
				writeInt16(out, totalRegions, index);

				writeInt32(out, totalPixels, index);

				return index;
			}

			static TexHeader fromBuffer(const uint8_t* buffer) noexcept {

				size_t index = 0;

				TexHeader header;

				header.width = readInt16(buffer, index);
				header.height = readInt16(buffer, index);
				header.totalRegions = readInt16(buffer, index);

				header.totalPixels = readInt32(buffer, index);

				return header;
			}

			void print() const noexcept {
				spdlog::info("Tex header");
				spdlog::info("Size: {}x{}", width, height);
				spdlog::info("Total regions: {}", totalRegions);
				spdlog::info("Total pixels: {}", totalPixels);
			}

			static constexpr size_t serializedSize() noexcept {
				return sizeof(uint16_t) * 3 + sizeof(uint32_t);
			}
		};

		struct TTXHeader {
			uint16_t width, height, totalTiles;
			uint32_t totalSize, lutSize;

			size_t serializeTo(uint8_t* out) const noexcept {
				size_t index = 0;

				writeInt16(out, width, index);
				writeInt16(out, height, index);
				writeInt16(out, totalTiles, index);

				writeInt32(out, totalSize, index);
				writeInt32(out, lutSize, index);

				return index;
			}

			static TTXHeader fromBuffer(const uint8_t* buffer) noexcept {

				size_t index = 0;

				TTXHeader header;

				header.width = readInt16(buffer, index);
				header.height = readInt16(buffer, index);
				header.totalTiles = readInt16(buffer, index);

				header.totalSize = readInt32(buffer, index);
				header.lutSize = readInt32(buffer, index);

				return header;
			}

			static constexpr size_t serializedSize() noexcept {
				return sizeof(uint16_t) * 3 + sizeof(uint32_t) * 2;
			}
		};

		struct TTXTileHeader {
			uint16_t frameLength, totalRegions;
			uint32_t totalPixels;

			size_t serializeTo(uint8_t* out) const noexcept {
				size_t index = 0;

				writeInt16(out, frameLength, index);
				writeInt16(out, totalRegions, index);

				writeInt32(out, totalPixels, index);

				return index;
			}

			static TTXTileHeader fromBuffer(const uint8_t* buffer) noexcept {

				size_t index = 0;

				TTXTileHeader header;

				header.frameLength = readInt16(buffer, index);
				header.totalRegions = readInt16(buffer, index);

				header.totalPixels = readInt32(buffer, index);

				return header;
			}

			static constexpr size_t serializedSize() noexcept {
				return sizeof(uint16_t) * 2 + sizeof(uint32_t);
			}
		};

		struct LutEntry {
			uint16_t id{};
			uint32_t offset{};
		};

	public:

		static void initialize();

		static bool encodeSTX(std::filesystem::path path, std::filesystem::path outputDir = "", std::string* outPath = nullptr);

		static graphics::Texture decodeSTX(std::filesystem::path path);

		static bool textureToSTX(graphics::Texture& tex, std::string name, std::filesystem::path outputDir = "", std::string* outPath = nullptr);

		static WavData decodeWav(std::filesystem::path path);

		static std::vector<graphics::Texture> decodeTileset(std::filesystem::path path, Vec2<size_t> gridSize);

		static bool encodeTTX(std::vector<graphics::Texture>& tiles, Vec2<size_t> size, std::string name, std::filesystem::path outputDir = "", std::string* outPath = nullptr);

		static std::vector<graphics::Texture> decodeTTX(std::filesystem::path path);
		
		// The directIndex bool determines if the sought tile is simply the id:th tile in the file (true), or if you want the tile with the specific id given (false)
		static graphics::Texture decodeTTXSingle(std::filesystem::path path, size_t id, bool directIndex);

		static std::vector<uint8_t>* getTTXLUT(std::filesystem::path path);

		static TTXHeader getTTXHeader(std::filesystem::path path);

		static bool encodeOled(std::filesystem::path imagePath, std::string name, std::filesystem::path outputDir, std::string* outPath = nullptr);

		static std::vector<uint8_t> decodeOled(std::filesystem::path path);

		static constexpr uint16_t NEXT_ROW_BYTE = std::numeric_limits<uint16_t>::max();
		static constexpr size_t MAX_SHAPE_SIZE = 8000;
		static constexpr size_t MAX_LUT_SIZE = 8000;

		static constexpr size_t MAX_WIDTH = 1920;
		static constexpr size_t MAX_HEIGHT = 1080;

	private:

		static int getOffsetFromLUT(std::ifstream& f, size_t offset, size_t lutSize, uint16_t id);

		template<typename T>
		static bool readBytes(std::ifstream& file, T& in);

		static std::vector<uint8_t> cropImage(unsigned char* image, Vec2<unsigned> imageSize, Vec2<unsigned> location, Vec2<unsigned> cropSize, unsigned channels);

		static graphics::Texture imageToTexture(std::filesystem::path path);

		static bool imageDataToBuffers(unsigned char* image, Vec2<unsigned> size, unsigned channels, size_t& pixelsWritten, size_t& totalRegions);

		static bool textureToBuffers(graphics::Texture& tex);

		static bool writeBuffersToSTX(std::filesystem::path name, std::filesystem::path outputDir, std::string* outName);

		static Shape decodeRegions(uint8_t* buffer, size_t textureHeight);

		static size_t packShape(uint8_t* buffer, graphics::Texture& texture);

		static void clearBuffers() noexcept;

		static RGB565 rgbaTo565(Pixel pixel);

		static void writeSignature(std::ofstream& f, std::string name, uint8_t version);

		static bool validateSignature(std::ifstream& f, std::string name, uint8_t version);

		static graphics::Texture textureFromStream(std::ifstream& f, Vec2<size_t> size, size_t totalPixels, size_t totalRegions);

		static std::vector<uint8_t> shapeBuffer;
		static std::vector<RGB565> bitmapBuffer;

		static std::vector<uint8_t> texHeaderBuffer;

		static std::vector<uint8_t> ttxHeaderBuffer;
		static std::vector<uint8_t> ttxTileHeaderBuffer;
		
		static std::vector<uint8_t> ttxLutBuffer;
	};
}