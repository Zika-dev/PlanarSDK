#pragma once

#include "Channel.hpp"

#include <array>
#include <utility>

namespace core::audio {

	enum class AudioChannel  {
		AUDIO_CHANNEL_1,
		AUDIO_CHANNEL_2,
		AUDIO_CHANNEL_3,
		AUDIO_CHANNEL_4,
		AUDIO_CHANNEL_5,
		AUDIO_CHANNEL_6,
		AUDIO_CHANNEL_7,
		AUDIO_CHANNEL_8,
		NUM_CHANNELS,
	};

	class Mixer {

	public:
		explicit Mixer(AudioEngine& engine);

		bool mixAllChannels(IAudioCallback::bufferType<uint8_t>& masterMixBuffer, size_t frames);

		void stopAllChannels();

		void pauseAllChannels();

		void resumeAllChannels();

		Channel& getChannel(AudioChannel channel) noexcept {
			return channels[static_cast<int>(channel)];
		}

		std::pair<Channel*, int> seekNextAvail() noexcept;

		double getMasterRMS() const noexcept {
			return masterRMS;
		}

		double getChannelRMS(AudioChannel channel) const noexcept {
			if (channels[static_cast<int>(channel)].isActive()) {
				return channels[static_cast<int>(channel)].getRMS();
			}
			else {
				return 0.0;
			}

			return channels[static_cast<int>(channel)].getRMS();
		}

		int setRMSPeriod(int period) noexcept {
			RMSPeriod = period;
		}

		IAudioCallback::bufferType<float>& getSampleHistory() noexcept {
			return sampleHistory;
		}

	private:
		double calculateRMS(double squareSum, int period) const noexcept;

		AudioEngine& engine;

		std::array<Channel, static_cast<size_t>(AudioChannel::NUM_CHANNELS)> channels;

		const char* TAG = "Mixer";

		double masterRMS = 0.0;
		double squareSumMaster = 0.0;
		std::array<double, static_cast<size_t>(AudioChannel::NUM_CHANNELS)> squareSumChannels{};
		int RMSPeriod = 320;
		int periodSinceLastRMS = 0;

		int sampleHistoryIndex = 0;
		int sampleHistorySize = 1024;
		IAudioCallback::bufferType<float> sampleHistory{};
	};
}