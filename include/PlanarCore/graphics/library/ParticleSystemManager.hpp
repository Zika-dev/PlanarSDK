#pragma once

#include "Core.hpp"
#include "ParticleSystem.hpp"
#include "Utils.hpp"
#include "graphics/driver/Rasterizer.hpp"
#include "Camera.hpp"
#include "Core.hpp"
#include "ParticleAttractor.hpp"
#include "SlotMap.hpp"

namespace core::graphics {

	class SDKGraphics;

	class PLANAR_API  ParticleSystemManager {
	public:

		SlotMap<ParticleSystem> particleSystems;
		SlotMap<ParticleAttractor> particleAttractors;

		ParticleSystemManager(SDKGraphics* consoleGraphics);

		Handle createParticleSystem(ParticleSystemConfig cfg, Vec2<float> startPos = {});

		ParticleSystem* peekParticleSystem(Handle handle);

		Handle createAttractor(const ParticleAttractorConfig& cfg);

		ParticleAttractor* peekAttractor(Handle handle);

		void removeParticleSystem(Handle handle);

		void removeAttractor(Handle handle);

		// Commands
		void setParticlesActive(Handle handle, bool active);

		void setParticlesPos(Handle handle, Vec2<float> pos);

		void setAttractorActive(Handle handle, bool active);

		void setAttractorPos(Handle handle, Vec2<float> pos);

		void setParticlesFrozen(Handle handle, bool frozen);

		void updateParticlesCfg(Handle handle, const ParticleSystemConfig& cfg);

		void updateAttractorCfg(Handle handle, const ParticleAttractorConfig& cfg);

		void updateAllParticles(float dt);

		void updateSingleParticles(Handle h, float dt);

		void drawGizmos(const Camera& cam);
		
		void setDrawGizmos(bool draw) noexcept {
			gizmos = draw;
		}

	private:
		SDKGraphics* consoleGraphics;
		
		int nextId = 0;

		bool gizmos = false;
	};
}