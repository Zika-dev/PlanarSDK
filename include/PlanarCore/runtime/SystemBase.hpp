#pragma once

#include <vector>

#include "GameObject.hpp"

namespace core::runtime {
	class SystemBase {
	public:
		virtual ~SystemBase() = default;

		virtual void init() {}
		virtual void shutdown() {}

		void registerObject(const GameObject& g) {
			// Add object if it qualifies and is not registered already
			auto it = std::find(subscribedObjects.begin(), subscribedObjects.end(), &g);
			if (accepts(g) && it == subscribedObjects.end()) {
				subscribedObjects.push_back(&g);
				CORE_LOG_INFO("System registered new object with ID: {}", g.getId());
			}
			else {
				CORE_LOG_WARN("Could not register object with ID {} for system: it does not meet the critera or is already registered", g.getId());
			}
		}

		void unregisterObject(const GameObject& g) {
			// Swap element with last element and pop back
			auto it = std::find(subscribedObjects.begin(), subscribedObjects.end(), &g);
			if (it == subscribedObjects.end()) return;

			std::iter_swap(it, subscribedObjects.end() - 1);
			subscribedObjects.pop_back();

			CORE_LOG_INFO("System unregistered object with ID: {}", g.getId());
		}

		virtual void update(float deltaTime) = 0;

	protected:
		virtual bool accepts(const GameObject& g) const { return true; }
		std::vector<const GameObject*> subscribedObjects;
	};
}