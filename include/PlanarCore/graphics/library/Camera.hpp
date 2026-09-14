#pragma once

#include <vector>
#include <memory>

#include "Core.hpp"
#include "SDL3/SDL.h"
#include "Utils.hpp"

namespace core::graphics {
	class PLANAR_API Camera {
	public:

		Vec2<float> targetPosition = { 0, 0 };
		float speed = 1.0f;

		Camera(Vec2<float> position, float speed = 1.0f);

		Camera() = default;

		void updatePosition(float deltaTime);

		void setPosition(Vec2<float> position) noexcept {
			targetPosition = position;
			this->position = position;
		}

		Vec2<float> getPosition() const noexcept {
			return position;
		}

		Vec2<float> getVelocity() const noexcept {
			return velocity;
		}

		Vec2<uint16_t> getScreenPos(Vec2<float> pos);

	private:
		Vec2<float> velocity{};
		Vec2<float> position{};
	};
}