#pragma once

#include "audio/core/IAudioCallback.hpp"
#include "AudioClip.hpp"
#include "SlotMap.hpp"
#include "AudioClipInfo.hpp"

namespace core::audio {

	class AudioEngine;

	class Channel {

	public:
		void init(AudioEngine* engine);

		void setSample(Handle handle);

		void setLinkedClip(AudioClipInfo clip);
		
		AudioClipInfo getLinkedClipInfo() {
			return audioClipInfo;
		}

		void setActive(bool active) noexcept {
			this->active = active;
		}

		bool isActive() const noexcept {
			return active;
		}

		bool isPlaying() const noexcept {
			return active && !paused;
		}

		void setPaused(bool paused) {
			this->paused = paused;
		}

		bool isPaused() const noexcept {
			return paused;
		}

		uint32_t getGeneration() const noexcept {
			return generation;
		}

		void makeFree();

		void getAudio(uint8_t* buf, size_t len);

		void setRMS(double RMS) noexcept {
			this->RMS = RMS;
		}

		double getRMS() const noexcept {
			return RMS;
		}

		IAudioCallback::bufferType<uint8_t>& getBuffer() {
			return buffer;
		}

	private:

		void fillBufferWithSamples(size_t sampleStart, size_t amount); // Samples with effects applied

		Handle sampleHandle;
		WavData* sample = nullptr;
		int samplePtr = 0;
		uint32_t sampleLen = 0;
		uint32_t bufferOffset = 0; // For when a single sample needs to be copied multiple times in the same frame (shorter than 320 bytes)

		AudioClipInfo audioClipInfo;
		bool reverse = false;

		AudioEngine* engine = nullptr;
		IAudioCallback::bufferType<uint8_t> buffer{};

		uint32_t generation = 0;

		bool active;
		bool paused = false;

		double RMS;
	};
}