#pragma once

#include <array>

#include "Core.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "IControllerAPI.hpp"

namespace core::controller {
	class PLANAR_API SDKController : public IControllerAPI {
	public:

		struct ControllerData {
			Vec2<float> joystickAxis;

			std::array<bool, 7> buttons{}; // Controller has 7 buttons: X, Y, A, B, START, SELECT, JOYSTICK

			float batteryPercentage{};
			float batteryChargeRate{};
			bool charging{};
		};

		enum class EventType {
			PULSE,
			SMOOTH_TO,
			NONE,
		};

		SDKController();

		void update(float dt) override;

		bool isHeld(Button button) const override;

		bool isReleased(Button button) const override;

		bool getsHeld(Button button) const override;

		bool getsReleased(Button button) const override;

		Vec2<float> getAxis() const override;

		// Motor
		void setMotorIntensity(float intensity) override;

		void motorLerpTo(float intensity, uint32_t time) override;

		void pulseMotorTriangle(float intensity, uint32_t time) override;

		void pulseMotorDecay(float intensity, uint32_t time) override;

		float getMotorIntensity() const override {
			return currentMotorIntensity;
		}

		// LED
		void setLedColor(RGB888 color) override;

		void ledLerpTo(RGB888 color, uint32_t time) override;

		void pulseLedTriangle(RGB888 color, uint32_t time) override;

		void pulseLedDecay(RGB888 color, uint32_t time) override;

		RGB888 getLedColor() const override {
			return currentColor;
		}

	private:

		void updateMotor(float dt);

		void updateLed(float dt);

		void keyboardToController();
		void gamepadToController();

		ControllerData currData;
		ControllerData lastData;

		// Motor
		Timer motorLerpTimer;
		float startMotorIntensity, endMotorIntensity;
		float currentMotorIntensity = 0.0f;
		bool motorPulseFalling = false;

		EventType motorEvent = EventType::NONE;

		// LED
		RGB888 currentColor;

		Timer ledLerpTimer;
		RGB888 startColor, endColor;
		float R, G, B; // For smooth lerping
		bool ledPulseFalling = false;

		EventType ledEvent = EventType::NONE;
	};
}
