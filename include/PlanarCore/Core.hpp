#pragma once

#include <SDL3/SDL.h>

#ifdef PLANAR_EXPORTS
#define PLANAR_API __declspec(dllexport)
#else
#define PLANAR_API __declspec(dllimport)
#endif

#ifdef _DEBUG
constexpr auto kBuildConfig = "Debug";
#else
constexpr auto kBuildConfig = "Release";
#endif

namespace core {
	bool PLANAR_API init();

	constexpr int WINDOW_WIDTH = 1920;
	constexpr int WINDOW_HEIGHT = 1080;

	// Game size
	constexpr int WIDTH = 320;
	constexpr int HEIGHT = 240;

	PLANAR_API extern SDL_Window* window;
	PLANAR_API extern SDL_Renderer* renderer;
	PLANAR_API extern SDL_Gamepad* gamepad;

	PLANAR_API extern float mainScale;
}