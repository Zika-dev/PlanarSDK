#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>
#include "Utils.hpp"

using ObjectId = uint16_t;

namespace core::tilemap {
	class PropertyBag {

		enum class PropertyType {
			Bool,
			Color,
			Float,
			Int,
			ObjectId,
			String
		};

		template<typename T>
		constexpr PropertyType propertyTypeOf();

		template<> constexpr PropertyType propertyTypeOf<bool>() { return PropertyType::Bool; }
		template<> constexpr PropertyType propertyTypeOf<RGB565>() { return PropertyType::Color; }
		template<> constexpr PropertyType propertyTypeOf<float>() { return PropertyType::Float; }
		template<> constexpr PropertyType propertyTypeOf<int>() { return PropertyType::Int; }
		template<> constexpr PropertyType propertyTypeOf<ObjectId>() { return PropertyType::ObjectId; }
		template<> constexpr PropertyType propertyTypeOf<std::string>() { return PropertyType::String; }

		struct Entry {
			PropertyType type;
			size_t id; // Index in corresponding vector
		};

		// Since bools are apparently a special case in vectors...
		struct Bool {

			Bool(bool val) : value(val) {}

			bool value;
			operator bool& () { return value; }
			operator bool() const { return value; }
		};

		std::unordered_map<std::string, Entry> lookup;

		// Properties
		std::vector<Bool> bools;
		std::vector<RGB565> colors;
		std::vector<float> floats;
		std::vector<int> ints;
		std::vector<ObjectId> objectIds;
		std::vector<std::string> strings;

		// Get vector from type
		template<typename T>
		auto& storageFor() {
			if constexpr (std::is_same_v<T, bool>) return bools;
			else if constexpr (std::is_same_v<T, Bool>) return bools;
			else if constexpr (std::is_same_v<T, RGB565>) return colors;
			else if constexpr (std::is_same_v<T, float>) return floats;
			else if constexpr (std::is_same_v<T, int>) return ints;
			else if constexpr (std::is_same_v<T, ObjectId>) return objectIds;
			else if constexpr (std::is_same_v<T, std::string>) return strings;
			else static_assert(!sizeof(T), "Non supported type!");
		}

	public:

		template<typename T>
		void insertProperty(std::string name, T property) {
			PropertyType type = propertyTypeOf<T>();
			auto& vec = storageFor<T>();

			// Insert if property doesn't already exist
			if (auto it = lookup.find(name); it == lookup.end()) {
				vec.push_back(property);
				lookup.emplace(name, Entry{ propertyTypeOf<T>(), static_cast<size_t>(vec.size() - 1) });
			}
			else {
				throw std::runtime_error("Cannot insert the same property twice: " + name);
			}
		}

		template<typename T>
		T getProperty(std::string name) {
			PropertyType type = propertyTypeOf<T>();
			auto& vec = storageFor<T>();

			if (auto it = lookup.find(name); it != lookup.end()) {

				if (type != it->second.type) {
					throw std::runtime_error("Incorrect property type: " + name);
				}

				T property = vec[lookup[name].id];
				return property;
			}

			throw std::runtime_error("Property doesn't exist: " + name);

			return T{};
		}

		template<typename T>
		T* tryGetProperty(std::string name) {
			PropertyType type = propertyTypeOf<T>();
			auto& vec = storageFor<T>();

			if (auto it = lookup.find(name); it != lookup.end()) {

				if (type != it->second.type) {
					throw std::runtime_error("Incorrect property type: " + name);
				}

				T* property = &vec[lookup[name].id];
				return property;
			}

			return nullptr;
		}

		// Bools...
		template<>
		bool getProperty(std::string name) {
			PropertyType type = PropertyType::Bool;
			auto& vec = storageFor<Bool>();

			if (auto it = lookup.find(name); it != lookup.end()) {

				if (type != it->second.type) {
					throw std::runtime_error("Incorrect property type: " + name);
				}

				bool property = vec[lookup[name].id];
				return property;
			}

			throw std::runtime_error("Property doesn't exist: " + name);

			return false;
		}

		template<>
		bool* tryGetProperty<bool>(std::string name) {
			PropertyType type = PropertyType::Bool;
			auto& vec = storageFor<Bool>();

			if (auto it = lookup.find(name); it != lookup.end()) {

				if (type != it->second.type) {
					throw std::runtime_error("Incorrect property type: " + name);
				}

				bool* property = &vec[lookup[name].id].value;
				return property;
			}

			return nullptr;
		}
	};
}