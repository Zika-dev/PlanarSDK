#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "Core.hpp"
#include "AnimationClip.hpp"
#include "runtime/GameObject.hpp"
#include "Utils.hpp"
#include "inplace_function.h"

namespace core::graphics {

	class PLANAR_API Animation {

	public:

		Animation() = default;

		Animation(AnimationClip clip, Vec2<int> offset = { 0, 0 }, bool autoPlay = true) :
			clip(std::move(clip)), offset(offset), autoPlay(autoPlay)
		{}

		bool isValid() const noexcept {
			return clip && !clip->empty();
		}

		void setClip(AnimationClip newClip) {
			clip = std::move(newClip);
			reset();
		}

		AnimationClip getClip() const noexcept {
			return clip;
		}

		void setCurrentFrame(int frame) noexcept {
			if (clip && frame >= 0 && frame < clip->getFrameCount()) {
				currentFrameIndex = frame;
			}
		}

		int getCurrentFrame() const noexcept {
			return currentFrameIndex;
		}

		int getTotalFrames() const noexcept {
			return clip ? clip->getFrameCount() : 0;
		}

		void reset() noexcept {
			currentFrameIndex = 0;
			currTime = 0;
			finished = false;
		}

		void setOffset(Vec2<int> offset) noexcept {
			this->offset = offset;
		}

		void setGlobalSpeed(float speed) noexcept {
			globalSpeedMul = speed;
		}

		void setAutoPlay(bool autoPlay) noexcept {
			this->autoPlay = autoPlay;
		}

		void play(float deltaTime, const runtime::ObjectRenderData& g);

		bool isLooping() const noexcept {
			return loop;
		}

		void setLoop(bool l) noexcept {
			loop = l;
		}

		bool onLoop() const noexcept {
			return currentFrameIndex == 0;
		}

		// Meaningful for non-looping clips: true once playback has reached the last frame.
		bool isFinished() const noexcept {
			return finished;
		}

		Vec2<uint16_t> getSize() const {
			return clip ? clip->getSize() : Vec2<uint16_t>{};
		}

		void addEventCallback(std::string name, stdext::inplace_function<void(), 32> cb) {
			eventCallbacks[std::move(name)] = cb;
		}

	private:
		AnimationClip clip;
		int currentFrameIndex = 0;
		bool finished = false;

		bool autoPlay = true;
		float currTime = 0;
		bool loop = false;

		Vec2<int> offset{};
		float globalSpeedMul = 1.0f;
		
		std::unordered_map<std::string, stdext::inplace_function<void(), 32>> eventCallbacks;
	};
}