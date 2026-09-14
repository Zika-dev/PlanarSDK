#pragma once

#include "Utils.hpp"
#include "ParticleAttractorConfig.hpp"

namespace core::graphics {
	class ParticleAttractor {
	public:

		ParticleAttractor(const ParticleAttractorConfig& cfg) : cfg(cfg) {
			pos = cfg.pos;
		}

		void setActive(bool active) noexcept {
			this->active = active;
		}

		Vec2<float> pos;

		Vec2<float> getAffectingVelocity(Vec2<float> particlePos) const {
			if (!active) return {};

			Vec2<float> delta = pos - particlePos;

			float distSq = delta.x * delta.x + delta.y * delta.y;
			float dist = std::sqrt(distSq);

			if (dist > cfg.endRange) return {};
			if (dist < cfg.startRange) return {};
			if (dist == 0.0f) return {};

			float magnitude = cfg.strength / dist;
			Vec2<float> direction = delta / dist;

			return direction * magnitude;
		}

		ParticleAttractorConfig getConfig() const noexcept {
			return cfg;
		}

		void applyConfig(ParticleAttractorConfig newCfg) {
			cfg = newCfg;
		}

	private:

		ParticleAttractorConfig cfg;

		bool active = true;
	};
}