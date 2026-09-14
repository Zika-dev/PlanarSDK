#pragma once

#include "graphics/platform/SDKGraphics.hpp"
#include "graphics/library/ParticleSystemManager.hpp"

#include "audio/platform/SDKAudio.hpp"

#include "controller/SDKController.hpp"

#include "runtime/physics/PhysicsEngine.hpp"

#include "SceneManager.hpp"
#include "GlobalResourceRegistry.hpp"
#include "ComponentEventBus.hpp"
#include "ComponentManager.hpp"

#include "Core.hpp"

#include <memory>

namespace core {

	struct PLANAR_API EngineContext {

		// Internal
		std::shared_ptr<runtime::ComponentEventBus> componentEventBus = std::make_shared<runtime::ComponentEventBus>();
		
		// User space
		// These can potentially have handles that outlive them (which is why they are shared pointers, the handles track their lifetime)
		std::shared_ptr<audio::SDKAudio> consoleAudio{};
		std::shared_ptr<graphics::SDKGraphics> consoleGraphics{};

		runtime::PhysicsEngine physicsEngine{};
		controller::SDKController controller{};
		runtime::SceneManager scene{};
		runtime::GlobalResourceRegistry resources{};
		runtime::ComponentManager components{ componentEventBus.get()};

		bool init() {
			consoleAudio = std::make_shared<audio::SDKAudio>();
			consoleGraphics = std::make_shared<graphics::SDKGraphics>();

			if (!consoleGraphics->initialize(core::renderer)) return false;

			const unsigned sampleRate = 32000;
			if (!consoleAudio->initialize(sampleRate)) return false;

			physicsEngine.init();

			return true;
		}

		void shutdown() {
			CORE_LOG_WARN("Shutting down engine context!");
			componentEventBus->shutdown();
		}
	};
}