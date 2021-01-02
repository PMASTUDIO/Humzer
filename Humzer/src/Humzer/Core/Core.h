#pragma once

#include <memory>
#include <filesystem>

#ifdef HUM_PLATFORM_WINDOWS
	#ifdef HUM_DYNAMIC_LINK
		#ifdef HUM_BUILD_DLL
			#define HUMZER_API __declspec(dllexport)
		#else
			#define HUMZER_API __declspec(dllimport)
		#endif
	#else	
		#define HUMZER_API
	#endif
#else
    #error Humzel Only supports Windows x64!
#endif

#ifdef HUM_DEBUG
	#ifdef HUM_PLATFORM_WINDOWS
		#define HUM_DEBUGBREAK() __debugbreak()
	#endif
	#define HUM_ENABLE_ASSERTS
#else
	#define HUM_DEBUGBREAK()
#endif

#define HUM_EXPAND_MACRO(x) x
#define HUM_STRINGIFY_MACRO(x) #x

#ifdef HUM_ENABLE_ASSERTS
	#define HUM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { HUM_CORE##type##ERROR(msg, __VA_ARGS__); HUM_DEBUGBREAK(); } }
	#define HUM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) HUM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define HUM_INTERNAL_ASSERT_NO_MSG(type, check) HUM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", HUM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define HUM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define HUM_INTERNAL_ASSERT_GET_MACRO(...) HUM_EXPAND_MACRO( HUM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HUM_INTERNAL_ASSERT_WITH_MSG, HUM_INTERNAL_ASSERT_NO_MSG) )

	#define HUM_ASSERT(...) HUM_EXPAND_MACRO( HUM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#else
	#define HUM_ASSERT(...)
#endif

namespace Humzer {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
