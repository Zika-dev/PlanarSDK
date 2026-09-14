#pragma once

#include <array>

namespace core::audio {
	class IAudioCallback {
	public:
		virtual ~IAudioCallback() = default;

		static constexpr int BUFFER_SIZE = 1024;

		template<typename T>
		using bufferType = std::array<T, BUFFER_SIZE>;

		virtual void renderAudio(bufferType<uint8_t>*, size_t frames) = 0;
	};
}