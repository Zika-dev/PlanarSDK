#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>

#include "Core.hpp"
#include "Log.hpp"
#include "ComponentRegistry.hpp"
#include "ComponentEventBus.hpp"

namespace core::runtime {
	class PLANAR_API ComponentManager {
	public:
		ComponentManager(ComponentEventBus* bus) : bus(bus) {}

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;

		ComponentManager(ComponentManager&&) noexcept = default;
		ComponentManager& operator=(ComponentManager&&) noexcept = default;

		template<typename T>
		ComponentRegistry<T>& get() {
			auto idx = std::type_index(typeid(T));
			auto it = registries.find(idx);
			if (it == registries.end()) {
				CORE_LOG_TRACE("Creating new {} registry", idx.name());
				it = registries.emplace(
					idx,
					std::unique_ptr<void, void(*)(void*)>(
						new ComponentRegistry<T>(*bus),
						[](void* ptr) {
							delete static_cast<ComponentRegistry<T>*>(ptr);
						}
					)
				).first;
			}

			return *static_cast<ComponentRegistry<T>*>(it->second.get());
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<void, void(*)(void*)>> registries;

		ComponentEventBus* bus;
	};
}
