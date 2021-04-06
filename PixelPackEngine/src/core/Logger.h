#pragma once

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

#endif // DEBUG
