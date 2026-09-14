#pragma once

#include "Core.hpp"

#include "graphics/core/IGraphicsAPI.hpp"
#include "graphics/driver/Rasterizer.hpp"
#include "graphics/library/Texture.hpp"
#include "graphics/library/AnimationClip.hpp"
#include "graphics/library/Animation.hpp"
#include "graphics/library/Camera.hpp"
#include "graphics/library/ParticleSystemManager.hpp"
#include "graphics/driver/RenderQueue.hpp"

#include "runtime/GameObject.hpp"

#include "codec/ConsoleCodec.hpp"

#include "graphics/driver/Packets.hpp"

#include "SlotMap.hpp"
#include "graphics/driver/RenderObjects.hpp"

namespace core::graphics {
    class PLANAR_API SDKGraphics : public IGraphicsAPI, public std::enable_shared_from_this<SDKGraphics> {
        public:
            bool initialize(SDL_Renderer* renderer) {
                rasterizer = std::make_unique<Rasterizer>(renderer);
                renderQueue = std::make_unique<RenderQueue>();
                camera = std::make_unique<Camera>();
                camera->speed = 3.0f;
                
                particleSystemManager = std::make_unique<ParticleSystemManager>(this);

				backgroundColor = hexTo565(0x87CEEB); // Pleasant blue color
				

                return true;
            }
            
            TextureHandle uploadTexture(Texture& texture) override;

            Texture loadTexture(std::filesystem::path path) override {
                return codec::ConsoleCodec::decodeSTX(path);
            }

            TextureHandle createTexture(std::filesystem::path path) override;

            AnimationClip createAnimationClip(std::filesystem::path path, Vec2<uint16_t> size) override;

            Animation createAnimation(std::filesystem::path path, Vec2<uint16_t> size, bool loop = false) override;

            void unloadTexture(Handle handle) override;

            // Particle system
            ParticleSystemHandle createParticleSystem(const ParticleSystemConfig& handle, Vec2<float> startPos = {}) override;

			void drawParticles(const ParticleSystemHandle& handle, Vec2<int16_t> pos, int16_t order) override;

			void setParticlesActive(const ParticleSystemHandle& handle, bool active) override;

			void setParticlesFrozen(const ParticleSystemHandle& handle, bool frozen) override;

			void updateParticles(const ParticleSystemHandle& handle, const ParticleSystemConfig& pCfg) override;

            void removeParticles(ParticleSystemHandle& handle) override;

            void updateAllParticles(float dt);

            // Particle attractor
            ParticleAttractorHandle createParticleAttractor(const ParticleAttractorConfig& aCfg) override;

			void setAttractorActive(const ParticleAttractorHandle& handle, bool active) override;

			void setAttractorPos(const ParticleAttractorHandle& handle, Vec2<float> pos) override;

            void updateAttractor(const ParticleAttractorHandle& handle, const ParticleAttractorConfig& cfg) override;

            void removeAttractor(ParticleAttractorHandle& handle) override;

            // Drawing
            void drawGameObject(const runtime::ObjectRenderData& gameObject) override;

			void drawLine(Line line) override;

			void drawRect(Rectangle rect) override;

			void drawCircle(Circle circle) override;

            // Editor
            SDL_Texture* consoleTextureToSDLTexture(Texture& texture);

            Camera& getCamera() override {
                return *camera;
            }

            void setBackgroundColor(RGB565 color) {
                backgroundColor = color;
            }

            void clearScreen() override {
                renderQueueSize += 1;
                rasterizer->clear(backgroundColor);
            }

            void sortRenderQueue();

            void endFrame() override {
                rasterizer->rasterizerToScreen();
                renderQueue->reset();

                // Calculate bytes transferred
				bytesTransferred += MetadataPacket::serializedSize();
				bytesTransferred += AckPacket::serializedSize();

                bytesTransferred += renderQueueSize;
                bytesTransferred += calcChunkTransferPacketBytes(renderQueueSize);

                renderQueueSize = 0;
            }

            RenderQueue& getRenderQueue() const noexcept {
                return *renderQueue;
            }

            Rasterizer& getRasterizer() const noexcept {
                return *rasterizer;
            }

            bool isDirty() const noexcept {
                return texturesDirty;
            }

            void clearDirty() noexcept {
                texturesDirty = false;
            }

            SlotMap<Texture>& getLoadedTextures() {
                return loadedTextures;
            }

            size_t getBytesTransferred() const noexcept {
                return bytesTransferred;
            }

            size_t getVideoMemoryUsage() const {
                return videoMemoryUsage;
            }

            void clearBytesTransferred() noexcept {
                bytesTransferred = 0;
            }

            ParticleSystemManager& getParticleSystemManager() {
                return *particleSystemManager.get();
            }

        private:

            // Helper function for calculating bytesTransferred
			size_t calcChunkTransferPacketBytes(size_t transferSize) {
				if (transferSize == 0) {
					return 0;
				}

                size_t transfers = ceil(static_cast<float>(transferSize) / MAX_TRANSFER_SIZE);
                return (ChunkPacket::serializedSize() + AckPacket::serializedSize()) * transfers;
			}

            std::unique_ptr<Rasterizer> rasterizer;
            std::unique_ptr<RenderQueue> renderQueue;
            std::unique_ptr<Camera> camera;
            std::unique_ptr<ParticleSystemManager> particleSystemManager;

            RGB565 backgroundColor;

            SlotMap<Texture> loadedTextures;
            size_t videoMemoryUsage = 0;

            bool texturesDirty = false;

            size_t bytesTransferred = 0; // Amount of bytes of data transferred this frame
            size_t renderQueueSize = 0;
    };
}