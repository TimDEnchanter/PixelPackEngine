#include "Logger.h"

#include "spdlog\sinks\stdout_color_sinks.h"

namespace PixelPack
{
	std::shared_ptr<spdlog::logger> Logger::sptr_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::sptr_ClientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] (%s:%#) %n: %v%$");
		sptr_EngineLogger = spdlog::stdout_color_mt("PixelPackEngine");
		sptr_EngineLogger->set_level(spdlog::level::trace);

		sptr_ClientLogger = spdlog::stdout_color_mt("Client");
		sptr_ClientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetEngineLogger()
	{
		return sptr_EngineLogger;
	}

	std::shared_ptr<spdlog::logger>& Logger::GetClientLogger()
	{
		return sptr_ClientLogger;
	}

}
