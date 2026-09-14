#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

#include "Core.hpp"

namespace core {
	class PLANAR_API Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getEditorLogger() { return editorLogger; }
		inline static std::shared_ptr<spdlog::logger>& getAppLogger() { return appLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> editorLogger;
		static std::shared_ptr<spdlog::logger> appLogger;
	};
}

// Log macros
#define CORE_LOG_CRITICAL(...) core::Log::getCoreLogger()->critical(__VA_ARGS__)
#define CORE_LOG_ERROR(...) core::Log::getCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_WARN(...) core::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_INFO(...) core::Log::getCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_TRACE(...) core::Log::getCoreLogger()->trace(__VA_ARGS__)

#define EDITOR_LOG_CRITICAL(...) core::Log::getEditorLogger()->critical(__VA_ARGS__)
#define EDITOR_LOG_ERROR(...) core::Log::getEditorLogger()->error(__VA_ARGS__)
#define EDITOR_LOG_WARN(...) core::Log::getEditorLogger()->warn(__VA_ARGS__)
#define EDITOR_LOG_INFO(...) core::Log::getEditorLogger()->info(__VA_ARGS__)
#define EDITOR_LOG_TRACE(...) core::Log::getEditorLogger()->trace(__VA_ARGS__)

#define APP_LOG_CRITICAL(...) core::Log::getAppLogger()->critical(__VA_ARGS__)
#define APP_LOG_ERROR(...) core::Log::getAppLogger()->error(__VA_ARGS__)
#define APP_LOG_WARN(...) core::Log::getAppLogger()->warn(__VA_ARGS__)
#define APP_LOG_INFO(...) core::Log::getAppLogger()->info(__VA_ARGS__)
#define APP_LOG_TRACE(...) core::Log::getAppLogger()->trace(__VA_ARGS__)