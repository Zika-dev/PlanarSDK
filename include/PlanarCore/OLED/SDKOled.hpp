#pragma once

#include "Core.hpp"
#include "IOledAPI.hpp"
#include "audio/engine/AudioEngine.hpp"

namespace core::oled {
	class PLANAR_API SDKOled : IOledAPI {

	public:

		void init(SDL_Renderer* renderer, audio::AudioEngine* audioEngine);

		void clearScreen() override;

		void putPixel(Vec2<uint16_t> pos) override;

		// Extremely Fast Line Algorithm by Po-Han Lin
		void drawLine(Vec2<int16_t> start, Vec2<int16_t> end) override;

		std::vector<uint8_t> loadOledFrames(std::filesystem::path path) override;

		void drawOledFrame(std::vector<uint8_t> pixels, uint16_t frame) override;

		void displayAudio() override;

		SDL_Texture& getRenderTexture() {
			return *bufferTex;
		}

	private:

		void bufferToTexture();

		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> bufferTex{ nullptr, &SDL_DestroyTexture };

		std::unique_ptr<std::array<RGB565, OLED_WIDTH * OLED_HEIGHT>> buffer;

		audio::AudioEngine* audioEngine;
	};
}
