#pragma once

#include <memory>

#include "Core.h"

#include <spdlog/spdlog.h>

namespace Humzer {
	class HUMZER_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#ifdef HUM_DEBUG

	// CORE LOG
	#define HUM_CORE_FATAL(...) ::Humzer::Log::GetCoreLogger()->critical(__VA_ARGS__)
	#define HUM_CORE_ERROR(...) ::Humzer::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define HUM_CORE_WARN(...) ::Humzer::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define HUM_CORE_INFO(...) ::Humzer::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define HUM_CORE_TRACE(...) ::Humzer::Log::GetCoreLogger()->trace(__VA_ARGS__)

	// CLIENT LOG
	#define HUM_CLIENT_FATAL(...) ::Humzer::Log::GetClientLogger()->critical(__VA_ARGS__)
	#define HUM_CLIENT_ERROR(...) ::Humzer::Log::GetClientLogger()->error(__VA_ARGS__)
	#define HUM_CLIENT_WARN(...) ::Humzer::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define HUM_CLIENT_INFO(...) ::Humzer::Log::GetClientLogger()->info(__VA_ARGS__)
	#define HUM_CLIENT_TRACE(...) ::Humzer::Log::GetClientLogger()->trace(__VA_ARGS__)

#else

	// CORE LOG
	#define HUM_CORE_FATAL(...)
	#define HUM_CORE_ERROR(...)
	#define HUM_CORE_WARN(...)
	#define HUM_CORE_INFO(...)
	#define HUM_CORE_TRACE(...)

	// CLIENT LOG
	#define HUM_CLIENT_FATAL(...)
	#define HUM_CLIENT_ERROR(...)
	#define HUM_CLIENT_WARN(...)
	#define HUM_CLIENT_INFO(...)
	#define HUM_CLIENT_TRACE(...)

#endif
