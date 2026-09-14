#pragma once

#include "Utils.hpp"

namespace core::controller {
	enum class Button {
		X,
		Y,
		A,
		B,
		START,
		SELECT,
		JOYSTICK,
	};

	class IControllerAPI {
	public:
		virtual void update(float dt) = 0;

		// Buttons
		virtual bool isHeld(Button button) const = 0;

		virtual bool isReleased(Button button) const = 0;

		virtual bool getsHeld(Button button) const = 0;

		virtual bool getsReleased(Button button) const = 0;

		// Joystick
		virtual Vec2<float> getAxis() const = 0;

		// Motor
		virtual void setMotorIntensity(float intensity) = 0;

		virtual void motorLerpTo(float intensity, uint32_t time) = 0;

		virtual void pulseMotorTriangle(float intensity, uint32_t time) = 0;

		virtual void pulseMotorDecay(float intensity, uint32_t time) = 0;

		virtual float getMotorIntensity() const = 0;

		// LED
		virtual void setLedColor(RGB888 color) = 0;

		virtual void ledLerpTo(RGB888 color, uint32_t time) = 0;

		virtual void pulseLedTriangle(RGB888 color, uint32_t time) = 0;

		virtual void pulseLedDecay(RGB888 color, uint32_t time) = 0;

		virtual RGB888 getLedColor() const = 0;
	};
}