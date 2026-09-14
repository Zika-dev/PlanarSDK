#pragma once

#include "SDKOled.hpp"

class OledTest {

public:

	OledTest(core::oled::SDKOled& oled);

	void draw(float dt);

private:

	void drawSineSequence();

	void drawAnimation(float dt);

	void drawAnimationRev(float dt);

	core::oled::SDKOled& oled;

	float elapsed = 0.0f;

	std::vector<uint8_t> animationFrames;
	uint32_t totalFrames = 0;
	uint32_t currentFrame = 0;

	float frameTime = 30.0f; // ms
	float timeSinceLastFrame = 0.0f;

};