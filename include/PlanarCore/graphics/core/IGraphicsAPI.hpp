#pragma once

#include <filesystem>

#include "graphics/library/Texture.hpp"
#include "runtime/GameObject.hpp"

#include "graphics/library/Camera.hpp"
#include "graphics/library/TextureHandle.hpp"
#include "graphics/library/AnimationClip.hpp"
#include "graphics/library/Animation.hpp"
#include "graphics/library/Shapes.hpp"
#include "graphics/library/ParticleSystemConfig.hpp"
#include "graphics/library/ParticleSystemHandle.hpp"
#include "graphics/library/ParticleAttractorConfig.hpp"
#include "graphics/library/ParticleAttractorHandle.hpp"

namespace core::graphics {
    class IGraphicsAPI {
        public:

            // Textures
            virtual TextureHandle uploadTexture(Texture& texture) = 0;

            virtual Texture loadTexture(std::filesystem::path path) = 0;

            virtual TextureHandle createTexture(std::filesystem::path path) = 0;

            virtual AnimationClip createAnimationClip(std::filesystem::path path, Vec2<uint16_t> size) = 0;

            virtual Animation createAnimation(std::filesystem::path path, Vec2<uint16_t> size, bool loop = false) = 0;

            virtual void unloadTexture(Handle handle) = 0;

			// Particle system
            virtual ParticleSystemHandle createParticleSystem(const ParticleSystemConfig& pCfg, Vec2<float> startPos = {}) = 0;

			virtual void drawParticles(const ParticleSystemHandle& handle, Vec2<int16_t> pos, int16_t order) = 0;

			virtual void setParticlesActive(const ParticleSystemHandle& handle, bool active) = 0;

			virtual void setParticlesFrozen(const ParticleSystemHandle& handle, bool frozen) = 0;

			virtual void updateParticles(const ParticleSystemHandle& handle, const ParticleSystemConfig& pCfg) = 0;

            virtual void removeParticles(ParticleSystemHandle& pCfg) = 0;

			// Particle attractor
			virtual ParticleAttractorHandle createParticleAttractor(const ParticleAttractorConfig& aCfg) = 0;

			virtual void setAttractorActive(const ParticleAttractorHandle& handle, bool active) = 0;

			virtual void setAttractorPos(const ParticleAttractorHandle& handle, Vec2<float> pos) = 0;

            virtual void updateAttractor(const ParticleAttractorHandle& handle, const ParticleAttractorConfig& cfg) = 0;

            virtual void removeAttractor(ParticleAttractorHandle& handle) = 0;

            // Drawing
            virtual void drawGameObject(const runtime::ObjectRenderData& gameObject) = 0;

            virtual void drawLine(Line line) = 0;

            virtual void drawRect(Rectangle rect) = 0;

            virtual void drawCircle(Circle circle) = 0;

            virtual Camera& getCamera() = 0;

            virtual void setBackgroundColor(RGB565 color) = 0;

            virtual void clearScreen() = 0;

            virtual void endFrame() = 0;
    };
}