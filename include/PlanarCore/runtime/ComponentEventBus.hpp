#pragma once

#include <vector>

#include "Core.hpp"

namespace core::runtime {

	class ComponentRegistryBase;

	class PLANAR_API ComponentEventBus {

	public:

		enum class EventType {
			OBJECT_REMOVED,
			OBJECT_COPIED,
		};

		void subscribe(ComponentRegistryBase* registry);

		void publishEvent(EventType type, int objectId, int copyTo = -1);

		void shutdown() {
			open = false;
		}

		bool isOpen() {
			return open;
		}

	private:
		std::vector<ComponentRegistryBase*> subscribers;
		bool open = true;
	};
}
