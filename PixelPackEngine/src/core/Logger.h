#pragma once

#include <spdlog\spdlog.h>
#include <spdlog\fmt\ostr.h>

namespace PixelPack
{

	class PXPK_API Logger
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetEngineLogger();
		static std::shared_ptr<spdlog::logger>& GetClientLogger();

	private:
		#pragma warning( push )
		#pragma warning( disable : 4251 ) // Disable C4251 for these lines. It's fine since these are private
		static std::shared_ptr<spdlog::logger> sptr_EngineLogger;
		static std::shared_ptr<spdlog::logger> sptr_ClientLogger;
		#pragma warning( pop )
	};

}

#ifdef _DEBUG

	// Engine log macros
	#define PXPK_LOG_ENGINE_TRACE(...)		SPDLOG_LOGGER_TRACE(PixelPack::Logger::GetEngineLogger(), __VA_ARGS__)
	#define PXPK_LOG_ENGINE_INFO(...)		SPDLOG_LOGGER_INFO(PixelPack::Logger::GetEngineLogger(), __VA_ARGS__)
	#define PXPK_LOG_ENGINE_WARN(...)		SPDLOG_LOGGER_WARN(PixelPack::Logger::GetEngineLogger(), __VA_ARGS__)
	#define PXPK_LOG_ENGINE_ERROR(...)		SPDLOG_LOGGER_ERROR(PixelPack::Logger::GetEngineLogger(), __VA_ARGS__)
	#define PXPK_LOG_ENGINE_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(PixelPack::Logger::GetEngineLogger(), __VA_ARGS__)

	// Client log macros
	#define PXPK_LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(PixelPack::Logger::GetClientLogger(), __VA_ARGS__)
	#define PXPK_LOG_INFO(...)		SPDLOG_LOGGER_INFO(PixelPack::Logger::GetClientLogger(), __VA_ARGS__)
	#define PXPK_LOG_WARN(...)		SPDLOG_LOGGER_WARN(PixelPack::Logger::GetClientLogger(), __VA_ARGS__)
	#define PXPK_LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(PixelPack::Logger::GetClientLogger(), __VA_ARGS__)
	#define PXPK_LOG_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(PixelPack::Logger::GetClientLogger(), __VA_ARGS__)

	// Assertion log macros
	#ifdef PXPK_PLATFORM_WINDOWS

		#define PXPK_ASSERT_ENGINE(x, ...) { if(!(x)) { PXPK_LOG_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define PXPK_VERIFY_ENGINE(x, ...) { if(!(x)) { PXPK_LOG_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define PXPK_ASSERT(x, ...) { if(!(x)) { PXPK_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define PXPK_VERIFY(x, ...) { if(!(x)) { PXPK_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	#else

		#define PXPK_ASSERT_ENGINE(x, ...)
		#define PXPK_VERIFY_ENGINE(x, ...) x
		#define PXPK_ASSERT(x, ...)
		#define PXPK_VERIFY(x, ...) x

	#endif // PXPK_PLATFORM_WINDOWS

#else

	// Engine log macros
	#define PXPK_LOG_ENGINE_TRACE(...)	
	#define PXPK_LOG_ENGINE_INFO(...)	
	#define PXPK_LOG_ENGINE_WARN(...)	
	#define PXPK_LOG_ENGINE_ERROR(...)	
	#define PXPK_LOG_ENGINE_CRITICAL(...)

	// Client log macros
	#define PXPK_LOG_TRACE(...)		
	#define PXPK_LOG_INFO(...)		
	#define PXPK_LOG_WARN(...)		
	#define PXPK_LOG_ERROR(...)		
	#define PXPK_LOG_CRITICAL(...)

	// Assertion log macros
	#define PXPK_ASSERT_ENGINE(x, ...)
	#define PXPK_VERIFY_ENGINE(x, ...) x
	#define PXPK_ASSERT(x, ...)
	#define PXPK_VERIFY(x, ...) x

#endif // DEBUG
