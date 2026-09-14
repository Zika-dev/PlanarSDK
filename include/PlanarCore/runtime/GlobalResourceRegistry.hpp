#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>
#include <optional>

#include "Core.hpp"
#include "runtime/physics/PhysicsEngine.hpp"

namespace core::runtime {

	class GameObject;

	class PLANAR_API GlobalResourceRegistry {

	public:

		GlobalResourceRegistry() {
			addLayerMask("Default", 0);
		}

		void addObjectName(std::string name, const GameObject& g);

		std::optional<int32_t> getObjectId(std::string name);

		void addOrderName(std::string name, int32_t order);

		std::optional<int32_t> getOrder(std::string name);

		void addLayerMask(std::string name, uint32_t mask);

		std::optional<PhysicsEngine::LayerMask> getLayerMask(std::string name);

		std::unordered_map<std::string, PhysicsEngine::LayerMask>& getAllLayerMasks();

		void setProjectDir(std::filesystem::path dir);

		std::filesystem::path getProjectDir();

	private:

		std::unordered_map<std::string, int32_t> objects;

		std::unordered_map<std::string, int32_t> orders;

		std::unordered_map<std::string, PhysicsEngine::LayerMask> layerMasks;
		uint32_t nextLayerMaskBit = 0;

		std::filesystem::path projectDir;

	};
}