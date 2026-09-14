#pragma once

#include <string>
#include "Utils.hpp"

namespace core::graphics {
	class Texture
	{

	public:
		Texture() = default;

		Texture(TextureDescriptor descriptor) {
			bitmap.resize(descriptor.bitmapSize);

			for (int i = 0; i < descriptor.bitmapSize; ++i) {
				bitmap[i] = descriptor.bitmapPtr[i];
			}

			size = descriptor.size;
			shape = descriptor.shape;
			totalPixels = descriptor.bitmapSize;
			regionCount = descriptor.regionCount;
		}

		Vec2<uint16_t> getSize() const noexcept {
			return size;
		}

		Shape getShape() const noexcept {
			return shape;
		}

		std::vector<RGB565>& getBitmap() noexcept {
			return bitmap;
		}

		void setHandle(int16_t handle) noexcept {
			this->handle = handle;
		}

		int16_t getHandle() const noexcept {
			return handle;
		}

		uint32_t getTotalPixels() const noexcept {
			return totalPixels;
		}

		uint16_t getRegionCount() const noexcept {
			return regionCount;
		}

		size_t getRegionSize() const noexcept {
			return (8 * regionCount);
		}

		bool isValid() const noexcept {
			return totalPixels > 0;
		}

		void setFrameLength(uint16_t length) noexcept {
			frameLength = length;
		}

		uint16_t getFrameLength() const noexcept {
			return frameLength;
		}

		size_t getMemoryUsage() {
			size_t bytes = 0;

			bytes += bitmap.size() * sizeof(RGB565);
			
			for (const auto& regions : shape) {
				bytes += regions.size() * sizeof(Region);
			}

			// Other variables are negligible
			
			return bytes;
		}

	private:

		std::vector<RGB565> bitmap;
		Vec2<uint16_t> size;
		Shape shape;

		uint32_t totalPixels = 0;
		uint16_t regionCount = 0;

		int16_t handle = -1;

		uint16_t frameLength = 0; // If used in an animation
	};
}
