#pragma once

#define _USE_MATH_DEFINES

#include <vector>

#include "Core.hpp"
#include "Utils.hpp"
#include "ParticleSystemConfig.hpp"
#include "Texture.hpp"
#include "ParticleAttractor.hpp"

namespace core::graphics {

	class ParticleSystemManager;

	class PLANAR_API ParticleSystem {

	public:
		struct Particle {
			bool active = false;

			float lifetime = 0.0f; // Milliseconds
			float invLifetime = 0.0f;
			float currentLifeTime = 0.0f; // Milliseconds

			Vec2<float> velocity;
			Vec2<float> position;

			float alpha{ 255 };
			RGB565 startColor;
			RGB565 endColor;
			RGB565 color{};

			Vec2<int> getRoundedPos() const {
				return Vec2<int>(position.x + 0.5f, position.y + 0.5f);
			}
		};

		ParticleSystem() = default;

		ParticleSystem(ParticleSystemManager* manager, Handle handle, Vec2<int> pos, ParticleSystemConfig config = ParticleSystemConfig{});

		// Create particle system from texture
		ParticleSystem(ParticleSystemManager* manager, Handle handle, Texture& tex, Vec2<int> pos, ParticleSystemConfig config = ParticleSystemConfig{});

		void update(float dt, std::vector<ParticleAttractor>& attractors);

		bool applyConfig(const ParticleSystemConfig& newConfig);

	private:

		void emit();

		Particle& getNextParticle();

		void applySpawnConditions(Particle& particle);

		Vec2<float> getSpawnPos();

		bool playing = true;
		bool frozen = false;
		bool shouldBeRemoved = false;

		ParticleSystemConfig config;
		ParticleSystemConfig subEmitterCfg;
		std::vector<Handle> subEmitters;

		std::vector<Particle> particlePool;

		Vec2<int> pos;

		int particleIndex = -1;

		int particlesAlive = 0;

		Vec2<float> velocity; // Velocity of whole particle system

		float timeSinceLastEmit{ 0.0f }; // Milliseconds

		// Precomputed values
		float particlesPerMillisecond{ 0.0f };
		float invFadeStartPercentage{ 0.0f };
		float invLifetime{ 0.0f };

		ParticleSystemManager* manager;
		Handle handle;

		// Bounding rectangle
		int16_t smallestX = 0;
		int16_t greatestX = 0;

		int16_t smallestY = 0;
		int16_t greatestY = 0;

		// Utility
		bool validateConfig(const ParticleSystemConfig& newConfig);

		void precomputeValues();

		void regenerate();

	public:

		int getIndex() const noexcept {
			return particleIndex;
		}

		int getParticlesAlive() const noexcept {
			return particlesAlive;
		}

		void setActive(bool active) noexcept {
			this->playing = active;
		}

		bool getActive() const noexcept {
			return playing;
		}

		void setFrozen(bool frozen) noexcept {
			this->frozen = frozen;
		}

		std::vector<Particle>& getParticlePool() noexcept {
			return particlePool;
		}

		ParticleSystemConfig getConfig() const noexcept {
			return config;
		}

		ParticleSystemConfig& getConfigUnsafe() noexcept { // Get reference of config
			return config;
		}

		bool getShouldBeRemoved() const noexcept {
			return shouldBeRemoved;
		}

		Handle getHandle() const noexcept {
			return handle;
		}

		void setPos(Vec2<int> newPos) noexcept {
			velocity = pos - newPos;
			pos = newPos;
		}

		Vec2<int> getPos() const noexcept {
			return pos;
		}

		std::vector<Handle>& getSubEmitters() {
			return subEmitters;
		}


		friend class ParticleSystemManager;
		friend class Rasterizer;
	};
}