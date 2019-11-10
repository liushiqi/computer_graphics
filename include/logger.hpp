#pragma once

#define SPDLOG_LEVEL_NAMES                                                                                                                           \
  { "trace", "debug", "info", "warn", "error", "fatal", "off" }
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#ifdef _MSC_VER
#define SPDLOG_FUNCTION __FUNCSIG__
#else
#define SPDLOG_FUNCTION __PRETTY_FUNCTION__
#endif

// clang-format off
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// clang-format on

#define trace(...) SPDLOG_TRACE(__VA_ARGS__)
#define debug(...) SPDLOG_DEBUG(__VA_ARGS__)
#define info(...) SPDLOG_INFO(__VA_ARGS__)
#define warn(...) SPDLOG_WARN(__VA_ARGS__)
#define critical(...) SPDLOG_CRITICAL(__VA_ARGS__)

void init_logger();
