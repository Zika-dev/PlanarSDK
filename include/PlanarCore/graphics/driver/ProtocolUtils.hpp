#pragma once

#include <cstdint>
#include <iostream>
#include <array>
#include <limits>
#include <string_view>

#include "Utils.hpp"
#include "graphics/library/Texture.hpp"

namespace core::graphics {
	inline constexpr std::array<uint32_t, 256> buildCrcTable(uint32_t poly = 0x9960034c) noexcept { // Calculate CRC-32 checksum LUT
		std::array<uint32_t, 256> table{};

		for (size_t i = 0; i < 256; ++i) {
			uint32_t crc = static_cast<uint32_t>(i << 24); // Load byte into top of register
			for (int j = 0; j < 8; ++j) {
				if (crc & 0x80000000) crc = (crc << 1) ^ poly; // If top of register is 1, XOR by polynomial
				else crc <<= 1;

				table[i] = crc;
			}
		}

		return table;
	}

	constexpr std::array<uint32_t, 256> crcTable = buildCrcTable();

	inline uint32_t crc32(const uint8_t* data, size_t lenBytes, uint32_t init = 0xFFFFFFFF) noexcept { // Calculate CRC-32 checksum from LUT
		uint32_t reg = init;
		for (size_t i = 0; i < lenBytes; ++i) {
			uint8_t idx = (reg >> 24) ^ data[i];
			reg = (reg << 8) ^ crcTable[idx];
		}

		return reg;
	}

	inline bool validateChecksum(uint8_t* data, size_t lenBytes, uint32_t checksum) noexcept {
		uint32_t crc = crc32(data, lenBytes);
		if (crc != checksum) {
			//ESP_LOGE("Validator", "Error detected! Polynomial division yielded: %u", unsigned(crc));
			return false;
		}
		else return true;
	}

	struct spiTransaction {
		size_t length;
		uint8_t* tx_buffer;
		uint8_t* rx_buffer;
	};

	enum class MessageType : uint8_t {
		NONE = 0,

		// Control codes
		ACK = 5,
		FINAL_ACK = 6,
		NACK = 7,
		SACK = 8,

		// Transmission types
		METADATA = 20,
		CHUNK = 25,
	};

	enum class NackReason : uint8_t {
		NONE = 0,
		MEMORY = 1,
		CHECKSUM = 2,
	};

	enum class Resource : uint8_t {
		NONE = 0,
		TEXTURE = 1,
		RENDER_QUEUE = 2,
	};

	inline std::string_view messageTypeToStr(MessageType type) noexcept {
		switch (type) {
		case MessageType::NONE:        return "NONE";
		case MessageType::ACK:         return "ACK";
		case MessageType::FINAL_ACK:   return "FINAL_ACK";
		case MessageType::NACK:        return "NACK";
		case MessageType::SACK:        return "SACK";
		case MessageType::METADATA:    return "METADATA";
		case MessageType::CHUNK:        return "CHUNK";
		}
		return "UNKNOWN";
	}

	inline std::string_view nackReasonToStr(NackReason type) noexcept {
		switch (type) {
		case NackReason::NONE:        return "NONE";
		case NackReason::MEMORY:      return "MEMORY";
		case NackReason::CHECKSUM: 	  return "CHECKSUM";
		}
		return "UNKNOWN";
	}

	inline std::string_view resourceToStr(Resource type) noexcept {
		switch (type) {
		case Resource::NONE:         return "NONE";
		case Resource::TEXTURE:      return "TEXTURE";
		case Resource::RENDER_QUEUE: return "RENDER_QUEUE";
		}
		return "UNKNOWN";
	}

	struct TextureHeader {
		Vec2<uint16_t> size{};
		uint32_t totalPixels{};
		uint16_t regionCount{};
		uint16_t regionSize{};

		TextureHeader() = default;

		TextureHeader(const Texture& texture) {
			size = texture.getSize();
			totalPixels = texture.getTotalPixels();
			regionCount = texture.getRegionCount();
			regionSize = texture.getRegionSize();
		}

		static constexpr size_t serializedSize() noexcept {
			return sizeof(size.x) + sizeof(size.y)
				+ sizeof(totalPixels) + sizeof(regionCount)
				+ sizeof(regionSize);
		}

		size_t serializeTo(uint8_t* out) const noexcept {

			size_t index = 0;

			writeInt16(out, size.x, index);
			writeInt16(out, size.y, index);
			writeInt32(out, totalPixels, index);
			writeInt16(out, regionCount, index);
			writeInt16(out, regionSize, index);

			return index;
		}

		static TextureHeader fromBuffer(const uint8_t* buffer) noexcept {

			size_t index = 0;

			TextureHeader header;

			header.size.x = readInt16(buffer, index);
			header.size.y = readInt16(buffer, index);
			header.totalPixels = readInt32(buffer, index);
			header.regionCount = readInt16(buffer, index);
			header.regionSize = readInt16(buffer, index);

			return header;
		}
	};

	constexpr uint16_t MAX_TRANSFER_SIZE = 4000;
	constexpr uint16_t NEXT_ROW_BYTE = std::numeric_limits<uint16_t>::max();
	constexpr uint32_t MAX_TEXTURE_SIZE = 1000000;
	
	constexpr uint8_t MAX_ATTEMPTS = 5;
}