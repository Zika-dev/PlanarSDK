#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "TextureHandle.hpp"
#include "Utils.hpp"

namespace core::graphics {

	// Shared animation data (frames, frame events)
	class AnimationClipRaw {
	public:
		AnimationClipRaw() = default;

		AnimationClipRaw(std::vector<TextureHandle> frames) :
			frames(std::move(frames))
		{}

		const std::vector<TextureHandle>& getFrames() const noexcept {
			return frames;
		}

		TextureHandle getFrame(int index) const {
			return frames[index];
		}

		int getFrameCount() const noexcept {
			return static_cast<int>(frames.size());
		}

		bool empty() const noexcept {
			return frames.empty();
		}

		Vec2<uint16_t> getSize() const {
			if (frames.empty()) return {};
			return frames[0].getSize();
		}

		void addFrameEvent(std::string name, uint32_t frame) {
			if (frame >= frames.size()) {
				CORE_LOG_WARN("Could not add frame event: event \"{}\" at frame {} exceeds (or is equal to) total frames {}!", name, frame, frames.size());
				return;
			}

			frameEvents[frame] = std::move(name);
		}

		const std::string& getFrameEvent(uint32_t frame) {
			static const std::string empty;

			if (auto it = frameEvents.find(frame); it != frameEvents.end()) {
				return it->second;
			}

			return empty;
		}

	private:
		std::vector<TextureHandle> frames;

		std::unordered_map<uint32_t, std::string> frameEvents;
	};

	using AnimationClip = std::shared_ptr<AnimationClipRaw>;
}