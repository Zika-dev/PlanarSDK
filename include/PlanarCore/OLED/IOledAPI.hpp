#pragma once

#include <filesystem>

#include "Utils.hpp"

namespace core::oled {

	constexpr int OLED_WIDTH = 128;
	constexpr int OLED_HEIGHT = 32;
	constexpr int OLED_PIXELS = OLED_WIDTH * OLED_HEIGHT;
	constexpr int OLED_BITS = OLED_WIDTH * OLED_HEIGHT / 8;

	class IOledAPI {
	public:

		virtual void clearScreen() = 0;

		virtual void putPixel(Vec2<uint16_t> pos) = 0;

		virtual void drawLine(Vec2<int16_t> start, Vec2<int16_t> end) = 0;

		virtual std::vector<uint8_t> loadOledFrames(std::filesystem::path path) = 0;

		virtual void drawOledFrame(std::vector<uint8_t> pixels, uint16_t frame) = 0;

		virtual void displayAudio() = 0;

	};
}
