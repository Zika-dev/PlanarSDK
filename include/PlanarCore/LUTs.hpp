#pragma once

#define _USE_MATH_DEFINES

#include <array>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace LUT {

	using trigArray = std::array<float, 361>; // 0 to 360 degrees


	inline trigArray makeSineLUT() {

		trigArray arr;

		for (int i = 0; i <= 360; ++i) {
			float sineVal = sin(i * M_PI / 180);
			arr[i] = sineVal;
		}

		return arr;
	}

	inline trigArray makeCosineLUT() {

		trigArray arr;

		for (int i = 0; i <= 360; ++i) {
			float cosVal = cos(i * M_PI / 180);
			arr[i] = cosVal;
		}

		return arr;
	}

	inline trigArray sine = makeSineLUT();
	inline trigArray cosine = makeCosineLUT();
}