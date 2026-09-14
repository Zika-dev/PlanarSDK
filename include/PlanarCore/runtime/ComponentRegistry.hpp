#pragma once

#include <unordered_map>
#include <memory>
#include <type_traits>

#include "Core.hpp"
#include "Log.hpp"
#include "GameObject.hpp"
#include "ComponentEventBus.hpp"

namespace core::runtime {

	class PLANAR_API ComponentRegistryBase {
	public:
		virtual ~ComponentRegistryBase() = default;
		virtual void onEvent(ComponentEventBus::EventType type, int objectId, int newObjectId = -1) = 0;
	};

	template<typename T>
	class ComponentRegistry : ComponentRegistryBase {

	public:

		ComponentRegistry(ComponentEventBus& bus) {
			bus.subscribe(this);
		}

		template<typename Derived, typename... Args>
		Derived* add(const GameObject& g, Args&&... args) {
			static_assert(std::is_base_of_v<T, Derived>, "Component is not of correct type!");

			if (!g.hasId()) {
				CORE_LOG_ERROR("Could not add component: game object doesn't have an ID!");
				return nullptr;
			}

			if (auto it = components.find(g.getId()); it != components.end()) {
				CORE_LOG_WARN("Could not add component: game object already has that component!");
				return nullptr;
			}

			CORE_LOG_TRACE("Adding component to object: {}", g.getId());

			auto component = std::make_unique<Derived>(std::forward<Args>(args)...);
			Derived* raw = component.get();

			components[g.getId()] = std::move(component);

			return raw;
		}

		T* get(const GameObject& g) {
			if (!g.hasId()) {
				CORE_LOG_ERROR("Could not get component: game object doesn't have an ID!");
				return nullptr;
			}

			return getInternal(g.getId());
		}

		T* get(int id) {
			return getInternal(id);
		}

		void remove(const GameObject& g) {
			if (!g.hasId()) {
				CORE_LOG_ERROR("Could not remove component: game object doesn't have an ID!");
				return nullptr;
			}

			CORE_LOG_TRACE("Removing component from object: {}", g.getId());

			removeInternal(g.getId());
		}

		void onEvent(ComponentEventBus::EventType type, int objectId, int copyTo = -1) override {
			if (type == ComponentEventBus::EventType::OBJECT_REMOVED) {
				removeInternal(objectId);
			}
			else if (type == ComponentEventBus::EventType::OBJECT_COPIED) {
				if constexpr (!T::cloneable) {
					CORE_LOG_WARN("Tried to clone component, but it is not cloneable.");
					return;
				}
				else 
					if (auto it = components.find(objectId); it != components.end()) {
						CORE_LOG_TRACE("Cloning component from {} to {}", objectId, copyTo);
						components[copyTo] = it->second->clone();
					}
			}
		}

	private:

		void removeInternal(int id) {
			if (components.erase(id)) {
				CORE_LOG_TRACE("Removed component from object: {}", id);
			}
		}

		T* getInternal(int id) {
			if (auto it = components.find(id); it != components.end()) {
				return it->second.get();
			}

			return nullptr;
		}

		std::unordered_map<int, std::unique_ptr<T>> components;

	};
}