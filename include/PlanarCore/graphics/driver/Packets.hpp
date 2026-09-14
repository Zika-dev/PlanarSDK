#pragma once

#include "ProtocolUtils.hpp"
#include <memory>

namespace core::graphics {

	inline const char* PROTOCOL_TAG = "Protocol";

	struct PacketHeader {
		uint16_t txID = 0; // Transaction ID

		uint32_t tick = 0;
		uint8_t ver = 2;
		MessageType type = MessageType::NONE;
		uint32_t crcChecksum = 0;

		static constexpr size_t serializedSize() noexcept {
			return sizeof(uint16_t) + sizeof(uint32_t) * 2
				+ sizeof(uint8_t) + sizeof(MessageType);
		}

		size_t serializeTo(uint8_t* out, size_t& index) noexcept;

		static PacketHeader deserializeFrom(const uint8_t* buf, size_t& index) noexcept;
		
		uint32_t calculateChecksum() noexcept;

		void print();
	};

	struct Packet {
		PacketHeader header;

		static constexpr MessageType TYPE = MessageType::NONE;

		Packet() = default;

		virtual ~Packet() = default;

		bool operator== (const Packet& rhs) const noexcept {
			return (header.type == rhs.header.type);
		}

		bool operator== (MessageType type) const noexcept {
			return (header.type == type);
		}

		static constexpr size_t serializedSize() noexcept {
			return PacketHeader::serializedSize();
		}

		virtual size_t serializeTo(uint8_t* out, size_t& index) noexcept = 0; // Serialize packet into buffer, return bytes written

		static std::unique_ptr<Packet> fromBuffer(const uint8_t* buffer, size_t& index);
	};

	struct MetadataPacket : public Packet {
		static constexpr MessageType TYPE = MessageType::METADATA;

		MetadataPacket(uint16_t txID, uint32_t tick);

		Resource resource = Resource::NONE;
		uint32_t totPayloadLen = 0;
		uint32_t maxChunkLen = MAX_TRANSFER_SIZE;

	private:
		uint16_t additionalDataSize = 0;
		std::vector<uint8_t> additionalData;

	public:

		static constexpr size_t serializedSize() noexcept {
			return PacketHeader::serializedSize() + sizeof(Resource) +
			sizeof(uint32_t) * 2 + sizeof(uint16_t);
		}

		std::vector<uint8_t>& getAdditionalData() {
			return additionalData;
		}

		uint16_t getAdditionalDataSize() {
			return additionalDataSize;
		}

		bool setAdditionalData(const uint8_t* data, uint16_t size) noexcept;

		size_t serializeTo(uint8_t* out, size_t& index) noexcept override;

		static MetadataPacket deserializeFrom(const uint8_t* buf, size_t& index) noexcept;
	};

	struct ChunkPacket : public Packet {
		static constexpr MessageType TYPE = MessageType::CHUNK;

		ChunkPacket(uint16_t txID, uint32_t tick);

	private:
		uint32_t crcChecksum = 0;
		std::vector<uint8_t> data;
		uint16_t payloadLen = 0;

	public:

		uint16_t chunkIdx = 0;

		static constexpr size_t serializedSize() noexcept {
			return PacketHeader::serializedSize() + sizeof(uint32_t) +
			sizeof(uint16_t) * 2;
		}

		bool setData(const uint8_t* data, uint16_t size) noexcept;

		uint32_t getChecksum() {
			return crcChecksum;
		}

		std::vector<uint8_t>& getData() {
			return data;
		}

		uint16_t getPayloadLen() {
			return payloadLen;
		}

		size_t serializeTo(uint8_t* out, size_t& index) noexcept override;

		static ChunkPacket deserializeFrom(const uint8_t* buf, size_t& index) noexcept;
	};

	struct AckPacket : public Packet {
		static constexpr MessageType TYPE = MessageType::ACK;

		AckPacket(uint16_t txID, uint32_t tick);

		int16_t additionalData = 0;

		static constexpr size_t serializedSize() noexcept {
			return PacketHeader::serializedSize() + sizeof(int16_t);
		}

		size_t serializeTo(uint8_t* out, size_t& index) noexcept override;

		static AckPacket deserializeFrom(const uint8_t* buf, size_t& index) noexcept;
	};

	struct NackPacket : public Packet {
		static constexpr MessageType TYPE = MessageType::NACK;

		NackPacket(uint16_t txID, uint32_t tick);

		NackReason reason = NackReason::NONE;

		static constexpr size_t serializedSize() noexcept {
			return PacketHeader::serializedSize() + sizeof(NackReason);
		}

		size_t serializeTo(uint8_t* out, size_t& index) noexcept override;

		static NackPacket deserializeFrom(const uint8_t* buf, size_t& index) noexcept;
	};
}