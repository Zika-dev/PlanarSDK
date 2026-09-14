#pragma once

#include <cstdint>
#include <memory>

#include "Core.hpp"
#include "Log.hpp"
#include "Utils.hpp"
#include "SlotMap.hpp"

namespace core::graphics {

	class IGraphicsAPI;

	struct PLANAR_API TextureHandle {
		TextureHandle() = default;

		static TextureHandle fromRaw(std::weak_ptr<IGraphicsAPI> gfx, Handle handle, Vec2<uint16_t> size, uint16_t frameTime);

		bool isValid() const noexcept {
			return (data && data->handle.valid());
		}

		Vec2<uint16_t> getSize() const noexcept {
			return size;
		}

		Handle getHandle() const noexcept {
			if (!data) {
				Handle h;
				h.id = -1;
				return h;
			}
			return data->handle;
		}

		uint16_t getFrameTime() const noexcept {
			return frameTime;
		}

		static constexpr size_t serializedSize() noexcept {
			return sizeof(uint16_t) * 4;
		}

	private:

		struct Data {
			std::weak_ptr<IGraphicsAPI> gfx;
			Handle handle;
		};

		Vec2<uint16_t> size;
		std::shared_ptr<Data> data;
		uint16_t frameTime = 0;
	};
}