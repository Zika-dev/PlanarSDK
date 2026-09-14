#pragma once

#include <utility>
#include <memory>

#include "Core.hpp"
#include "SlotMap.hpp"

namespace core::graphics {

	class IGraphicsAPI;

	class PLANAR_API ParticleSystemHandle {

	public:

		ParticleSystemHandle() = default;

		ParticleSystemHandle(const ParticleSystemHandle&) = delete;
		ParticleSystemHandle& operator=(const ParticleSystemHandle&) = delete;

		ParticleSystemHandle(ParticleSystemHandle&& other) noexcept : handle(std::exchange(other.handle, Handle{})), gfx(other.gfx) {

		}

		ParticleSystemHandle& operator=(ParticleSystemHandle&& other) noexcept {
			if (this != &other) {
				reset();

				handle = std::exchange(other.handle, Handle{});
				gfx = other.gfx;
			}

			return *this;
		}

		~ParticleSystemHandle() {
			reset();
		}

		void reset(Handle _handle = {});

		Handle get() const noexcept {
			return handle;
		}

		void setGfx(std::weak_ptr<IGraphicsAPI> gfx) {
			this->gfx = gfx;
		}

		bool isUploaded() const {
			return handle.valid();
		}

		void release() {
			handle.id = -1;
		}


	private:

		Handle handle;

		std::weak_ptr<IGraphicsAPI> gfx;

	};
}
