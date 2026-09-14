#pragma once

#include "Utils.hpp"

namespace core::graphics {
	class ParticleAttractorConfig {
	public:

		ParticleAttractorConfig(float strength = 5.0f, float endRange = 500, float startRange = 2) : strength(strength), endRange(endRange), startRange(startRange) {

		}

		Vec2<float> pos;

		float strength = 5.0;

		float endRange = 500.0f;
		float startRange = 2.0f;
	};
}