#pragma once

#include <SDL3/SDL_audio.h>

#include "Core.hpp"
#include "audio/core/IAudioCallback.hpp"

namespace core::audio {

	class PLANAR_API AudioDriver {

	public:

		AudioDriver(int sampleRate = 32000);

		bool start();

		void setOutputVolume(float volume);

		bool getStatus() const noexcept {
			return running;
		}

		void registerAudioCallback(IAudioCallback* cb);

		IAudioCallback::bufferType<uint8_t>* getReadyBuf() {
			return readyBuf;
		}

	private:
		static void SDLCALL fillAudioStream(void* userdata, SDL_AudioStream* astream, int additional_amount, int total_amount);

		void fillNextBuffer(int frames);

		const int CHANNELS = 1;
		int sampleRate = 32000;

		SDL_AudioSpec audioSpec;

		SDL_AudioStream* audioStream = nullptr;

		bool initialized = false;
		bool running = false;

		IAudioCallback::bufferType<uint8_t> audioBuf1;
		IAudioCallback::bufferType<uint8_t> audioBuf2;
		IAudioCallback::bufferType<uint8_t>* readyBuf;

		bool currBuf = false;

		IAudioCallback* audioCallback = nullptr;

		struct FillCallbackCtx {
			AudioDriver* driver;
		};
	};
}