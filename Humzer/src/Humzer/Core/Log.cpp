#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

void Humzer::Log::Init()
{
	// GLOBAL PATTERN IS MADE OF: BEGIN COLOR RANGE (%^) - [ TIME (%T) ] - LOGGER NAME (%n) - MESSAGE (%v) - END COLOR RANGE (%$)
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_CoreLogger = spdlog::stdout_color_mt("HUMZER");
	s_CoreLogger->set_level(spdlog::level::trace);

	s_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
	s_ClientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Humzer::Log::s_CoreLogger;

std::shared_ptr<spdlog::logger> Humzer::Log::s_ClientLogger;
