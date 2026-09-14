#pragma once

#include "Core.hpp"

namespace core::runtime {
	class PLANAR_API Scene {
	public:
		virtual ~Scene() = default;

		virtual void onLoad() = 0;
		virtual void onUpdate(float dt) = 0;
		virtual void onExit() {}
		virtual void onPause() {}
		virtual void onResume() {}
	};
}
