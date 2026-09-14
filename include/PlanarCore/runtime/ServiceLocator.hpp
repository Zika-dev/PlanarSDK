#pragma once

#include "Core.hpp"
#include "EngineContext.hpp"

namespace core {
	class PLANAR_API Services {
	public:
		static void provide(EngineContext* context) {
			ctx = context;
		}

		static bool ready() noexcept {
			return (ctx != nullptr);
		}

		// Internal
		static std::shared_ptr<runtime::ComponentEventBus> componentEventBus() { return ctx->componentEventBus; }

		// User space
		static audio::IAudioAPI& audio() { return *ctx->consoleAudio; }
		static graphics::IGraphicsAPI& graphics() { return *ctx->consoleGraphics; }
		static runtime::PhysicsEngine& physics() { return ctx->physicsEngine; }
		static controller::IControllerAPI& controller() { return ctx->controller; }
		static runtime::SceneManager& scene() { return ctx->scene; }
		static runtime::GlobalResourceRegistry& resources(){ return ctx->resources; }
		static runtime::ComponentManager& components() { return ctx->components; }

	private:
		inline static EngineContext* ctx = nullptr;
	};
}