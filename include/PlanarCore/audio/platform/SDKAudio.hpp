#pragma once

#include <memory>

#include "Core.hpp"
#include "audio/engine/AudioEngine.hpp"
#include "audio/core/IAudioAPI.hpp"
#include "audio/driver/AudioDriver.hpp"

namespace core::audio {
	class PLANAR_API SDKAudio : public IAudioAPI, public std::enable_shared_from_this<SDKAudio> {
	public:
		bool initialize(int sampleRate) override {
			engine = std::make_unique<AudioEngine>();
			driver = std::make_unique<AudioDriver>(sampleRate);
			driver->registerAudioCallback(engine.get());
			driver->start();

			return driver->getStatus();
		}

		bool isReady() override {
			return driver->getStatus();
		}

		AudioClip createClip(std::filesystem::path path) override {
			Handle handle = engine->loadSample(path);
			AudioClip clip = AudioClip::fromRaw(weak_from_this(), handle);

			return clip;
		}

		void unloadClip(Handle handle, uint8_t channel, uint32_t gen) override {
			engine->unloadSample(handle, channel, gen);
		}

		void playClip(AudioClip& clip, float pitchRange = 0.0f, AudioChannel channel = AudioChannel::NUM_CHANNELS) override
		{
			engine->playClip(clip, pitchRange, channel);
		}

		void stopClip(AudioClip& clip) override {
			engine->stopClip(clip);
		}

		void pauseClip(AudioClip& clip) override {
			engine->pauseClip(clip);
		}

		void resumeClip(AudioClip& clip) override {
			engine->resumeClip(clip);
		}

		void updateClip(AudioClip& clip) override {
			engine->updateClip(clip);
		}

		void stopAll() override {
			engine->stopAll();
		}

		void pauseAll() override {
			engine->pauseAll();
		}

		void resumeAll() override {
			engine->resumeAll();
		}

		AudioEngine& getEngine() {
			return *engine;
		}

		AudioDriver& getDriver() {
			return *driver;
		}

	private:
		std::unique_ptr<AudioDriver> driver;
		std::unique_ptr<AudioEngine> engine;
	};
}