#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Animation.hpp"
#include "AnimationClip.hpp"
#include "runtime/GameObject.hpp"
#include "Log.hpp"

namespace core::graphics {

	// Owns a set of Animations and can switch between them
	class Animator {
	public:
		Animator() = default;

		Animator(const Animator&) = delete;
		Animator& operator=(const Animator&) = delete;
		Animator(Animator&&) noexcept = default;
		Animator& operator=(Animator&&) noexcept = default;

		~Animator() = default;

		void addAnimation(const std::string& name, Animation animation) {
			animations[name] = animation;
		}

		// Switch current animation
		void play(const std::string& name, bool restart = false) {
			if (!restart && name == currentName) {
				return;
			}

			auto it = animations.find(name);
			if (it == animations.end()) {
				CORE_LOG_ERROR("Could not play animation: no animation named '{}'", name);
				return;
			}

			currentName = name;
			current = &it->second;
			current->reset();
		}

		void update(float dt, const runtime::ObjectRenderData& g) {
			if (current && current->isValid()) current->play(dt, g);
		}

		const std::string& getCurrentName() const noexcept {
			return currentName;
		}

		Animation* getCurrent() noexcept {
			return current;
		}

		Animation* getAnimation(std::string name) {
			if (auto it = animations.find(name); it != animations.end()) {
				return &it->second;
			}

			return nullptr;
		}

		bool isFinished() const noexcept {
			return current->isFinished();
		}

	private:
		std::unordered_map<std::string, Animation> animations;

		Animation* current = nullptr;
		std::string currentName;
	};
}