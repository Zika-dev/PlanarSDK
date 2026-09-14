#pragma once

#include "Core.hpp"
#include "audio/core/IAudioCallback.hpp"
#include "Mixer.hpp"
#include "AudioClip.hpp"
#include "SlotMap.hpp"

#include <filesystem>
#include <mutex>

namespace core::audio {
	class PLANAR_API AudioEngine : public IAudioCallback {

	public:

		AudioEngine();

		void renderAudio(IAudioCallback::bufferType<uint8_t>* buf, size_t frames) override;

		Handle loadSample(std::filesystem::path path);

		WavData* getSample(Handle handle);

		void unloadSample(Handle handle, uint8_t channel, uint32_t gen);

		void playClip(AudioClip& clip, float pitchRange = 0.0f, AudioChannel channel = AudioChannel::NUM_CHANNELS);

		void stopClip(AudioClip& clip);

		void pauseClip(AudioClip& clip);

		void resumeClip(AudioClip& clip);

		void updateClip(AudioClip clip);

		void stopAll();

		void pauseAll();

		void resumeAll();

		Channel& getChannel(AudioChannel channel);

		double getMasterdBFS() const;

		double getChanneldBFS(AudioChannel channel) const;

		int getNumActiveChannels();

		int getNumLoadedClips() noexcept;

		int getLoadedBytes() const noexcept;

		bufferType<float>& getSampleHistory() noexcept;

		std::mutex& getSampleMutex() noexcept {
			return sampleMutex;
		}

	private:

		uint32_t getNextChannelGen();

		Channel* getClipChannel(const AudioClip& clip);

		double RMSTodBFS(double RMS) const noexcept;

		Mixer mixer;

		bufferType<uint8_t> audioBuffer;
		uint32_t nextChannelGen = 0;

		SlotMap<WavData> loadedSamples;
		std::mutex sampleMutex; // For not loading/unloading samples while they are being rendered

		size_t loadedBytes = 0;
	};
}