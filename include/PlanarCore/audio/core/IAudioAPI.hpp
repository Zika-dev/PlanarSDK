#pragma once

#include <filesystem>
#include "audio/engine/AudioClip.hpp"
#include "audio/engine/Mixer.hpp"

namespace core::audio {
	class IAudioAPI {
	public:
		virtual bool initialize(int sampleRate) = 0;

		virtual bool isReady() = 0;

		virtual AudioClip createClip(std::filesystem::path path) = 0;

		virtual void unloadClip(Handle handle, uint8_t channel, uint32_t gen) = 0;

		virtual void playClip(AudioClip& clip, float pitchRange = 0.0f, AudioChannel channel = AudioChannel::NUM_CHANNELS) = 0;

		virtual void stopClip(AudioClip& clip) = 0;

		virtual void pauseClip(AudioClip& clip) = 0;

		virtual void resumeClip(AudioClip& clip) = 0;

		virtual void updateClip(AudioClip& clip) = 0;

		virtual void stopAll() = 0;

		virtual void pauseAll() = 0;

		virtual void resumeAll() = 0;

		virtual ~IAudioAPI() = default;
	};
}