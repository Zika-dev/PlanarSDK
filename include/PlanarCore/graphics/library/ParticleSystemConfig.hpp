#pragma once

#include <cstdint>
#include "Utils.hpp"
#include "TextureHandle.hpp"
#include "Log.hpp"
#include "SlotMap.hpp"

namespace core::graphics {
	enum class EmitterMode {
		CONTINUOUS,
		BURST,
		SINGLE
	};

	enum class SpawnShape {
		CIRCLE,
		RECTANGLE,
	};

	class ParticleSystemConfig {

	public:

		ParticleSystemConfig() = default;

		void setSize(uint32_t size) {
			if (uploaded) {
				CORE_LOG_WARN("Cannot resize particle system after it has been uploaded!");
				return;
			}

			this->size = size;
		}

		uint32_t getSize() const noexcept {
			return size;
		}

		// Emitter
		EmitterMode mode = EmitterMode::CONTINUOUS;

		uint16_t particlesPerSecond = 100;

		uint16_t burstInterval = 1000; // Milliseconds

		uint16_t burstAmount = 10;

		SpawnShape spawnShape = SpawnShape::CIRCLE;

		// For circular spawn shape
		float spawnRadius = 2.0f;

		// For rectangle spawn shape
		uint16_t rectWidth = 50;
		uint16_t rectHeight = 50;

		Vec2<float> startVelocity; // m/s

		float startVelocitySpread = 0.2f; // Random spread intensity

		// Particles
		bool randomLifetime = false;

		uint16_t particleLifetimeMin = 500; // Milliseconds
		uint16_t particleLifetimeMax = 1000; // Milliseconds

		Vec2<float> acceleration; // Can be used for gravity, wind etc. m/s^2

		float dampening = 0.0f; // Can be used for air resistance

		bool affectedByAttractors = true;

		bool useParticleTexture = false;

		bool createFromTexture = false;
		bool startColorFromTexture = true;

		TextureHandle particleTexture; // If createFromTexture is set, this will be the texture used

		BlendMode blendMode = BlendMode::NORMAL;

		bool colorOverLifetime = false;

		RGB565 startColor = static_cast<RGB565>(0xFFFF);
		RGB565 endColor = static_cast<RGB565>(0xFFFF);

		uint8_t startAlpha = 255;
		uint8_t endAlpha = 255;



		// RGP Specific
		bool valid = false;
		Vec2<uint16_t> texSize;
		bool isSubSystem = false;
		Handle masterSystem;

	private:
		uint32_t size = 500; // Total particles available to system
		bool uploaded = false;

		// Will be added back in a later version (too lazy to fix allat now)
		bool useSubEmitter = false;
		Handle subEmitter;

		friend class ParticleSystem;
	};
}
