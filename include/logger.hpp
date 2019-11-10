#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

#define trace(...) SPDLOG_TRACE(__VA_ARGS__)
#define debug(...) SPDLOG_DEBUG(__VA_ARGS__)
#define info(...) SPDLOG_INFO(__VA_ARGS__)
#define warn(...) SPDLOG_WARN(__VA_ARGS__)
#define critical(...) SPDLOG_CRITICAL(__VA_ARGS__)

void init_logger();
