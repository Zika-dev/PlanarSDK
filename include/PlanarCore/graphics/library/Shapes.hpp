#pragma once

#include "Utils.hpp"
#include <array>

namespace core::graphics {
	struct Line {
		Vec2<int16_t> start, end;
		RGB565 color;
		uint8_t alpha = 255;
		int16_t order = 0;
		bool isStatic = false;
	};

	struct Rectangle {
		Vec2<int16_t> pos;
		int16_t width, height;
		RGB565 color;
		uint8_t alpha = 255;
		bool filled = true;
		int16_t order = 0;
		bool isStatic = false;
	};

	struct Circle {
		Vec2<int16_t> midPoint;
		int16_t radius;
		RGB565 color;
		uint8_t alpha = 255;
		bool filled = true;
		int16_t order = 0;
		bool isStatic = false;
	};
}
