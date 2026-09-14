#pragma once

#include <memory>

#include "Core.hpp"
#include "SlotMap.hpp"

namespace core::audio {

	class IAudioAPI;

	class PLANAR_API AudioClip {

	public:
		AudioClip() = default;

		static AudioClip fromRaw(std::weak_ptr<IAudioAPI> audio, Handle handle);

		float volume = 1.0f;
		bool loop = false;
		float speed = 1.0f;

		bool isValid() const noexcept {
			return (data && data->handle.valid());
		}

		Handle getHandle() const noexcept {
			if (!data) {
				Handle h;
				h.id = -1;
				return h;
			}
			return data->handle;
		}

		uint8_t getCurrentChannel() const noexcept {
			return data->currentChannel;
		}

	private:

		struct Data {
			std::weak_ptr<IAudioAPI> audio;
			Handle handle;

			uint8_t currentChannel;
			uint32_t gen = 0;
		};

		std::shared_ptr<Data> data;

		friend class AudioEngine;
	};
}
