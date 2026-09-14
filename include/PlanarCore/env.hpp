#pragma once

#include "runtime/ServiceLocator.hpp"
#include "runtime/GlobalResourceRegistry.hpp"
#include "runtime/ComponentTypes.hpp"
#include "runtime/ComponentManager.hpp"
#include "runtime/Tag.hpp"

namespace gfx = core::graphics;
namespace rt = core::runtime;
namespace aud = core::audio;
namespace ctrl = core::controller;

namespace env {
	inline auto& graphics() { return core::Services::graphics(); }
	inline auto& audio() { return core::Services::audio(); }
	inline auto& physics() { return core::Services::physics(); }
	inline auto& controller() { return core::Services::controller(); }
	inline auto& scene() { return core::Services::scene(); }
	inline auto& resources() { return core::Services::resources(); }
	inline auto& components() { return core::Services::components(); }
}