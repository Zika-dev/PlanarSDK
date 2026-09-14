#pragma once

#include "Core.hpp"
#include "Scene.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

namespace core::runtime {

	using SceneCreator = std::function<std::unique_ptr<Scene>()>;

	class PLANAR_API SceneManager {
	public:

		SceneManager() = default;
		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		SceneManager(SceneManager&&) noexcept = default;
		SceneManager& operator=(SceneManager&&) noexcept = default;

		template<typename T>
		void insert(uint32_t id);

		template<typename T>
		void insert(std::string name);

		template<typename T>
		void insert(uint32_t id, std::string name);

		void replace(uint32_t id);
		void replace(std::string name);

		void push(uint32_t id);
		void push(std::string name);

		void pop();

		void set(uint32_t id);
		void set(std::string name);

		void update(float dt);

		bool anySceneLoaded();

		void unloadAll();

	private:

		SceneCreator* getScene(uint32_t id);
		SceneCreator* getScene(std::string name);

		void replaceImpl(SceneCreator& scene);
		void pushImpl(SceneCreator& scene);
		void setImpl(SceneCreator& scene);

		std::vector<std::unique_ptr<Scene>> stack;
		std::unordered_map<uint32_t, SceneCreator> sceneLookupId;
		std::unordered_map<std::string, SceneCreator> sceneLookupStr;
	};

	template<typename T>
	void SceneManager::insert(uint32_t id)
	{
		sceneLookupId[id] = []() {
			return std::make_unique<T>();
			};
	}

	template<typename T>
	void SceneManager::insert(std::string name)
	{
		sceneLookupStr[name] = []() {
			return std::make_unique<T>();
			};
	}

	template<typename T>
	void SceneManager::insert(uint32_t id, std::string name)
	{
		insert<T>(id);
		insert<T>(name);
	}
}