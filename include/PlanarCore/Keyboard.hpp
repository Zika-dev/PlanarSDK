#pragma once
#include <SDL3/SDL.h>
#include <string>

#include "Core.hpp"

class PLANAR_API Keyboard
{
public:
	Keyboard();

	void update(const SDL_Event& event);
	bool isHeld(unsigned char key) noexcept;
	bool isReleased(unsigned char key) noexcept;
	bool getsHeld(unsigned char key) noexcept;
	bool getsReleased(unsigned char key) noexcept;
	int stringToKey(std::string key) const noexcept;

private:
	bool keyStates[256];

	int lastKey[256];
};

PLANAR_API extern Keyboard keyboard;