#pragma once

#include <cstdint>

namespace core::runtime {

	constexpr static uint32_t defaultHash = 2166136261u;

	constexpr uint32_t hashTag(const char* str, uint32_t hash = defaultHash) {
		return (*str == '\0') ? hash : hashTag(str + 1, (hash ^ (uint32_t)*str) * 16777619u);
	}

	// Hashed string
	struct Tag {

		constexpr Tag() : hash(defaultHash) {}
		constexpr Tag(const char* str) : hash(hashTag(str)) {}

		bool operator==(const Tag& other) const {
			return hash == other.hash;
		}

		bool empty() const noexcept {
			return hash == defaultHash;
		}

		uint32_t getHash() const noexcept {
			return hash;
		}

	private:

		uint32_t hash = 0;
	};
}
