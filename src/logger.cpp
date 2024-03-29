#include <logger.hpp>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void init_logger() {
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(spdlog::level::info);

  auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/trace.log", 10 * 1024 * 1024, 64, true);
  file_sink->set_level(spdlog::level::trace);
  std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("homework", spdlog::sinks_init_list({console_sink, file_sink}));
  logger->set_level(spdlog::level::trace);
  spdlog::set_default_logger(logger);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%t] [%5l] [%@] [%!] %v");
  spdlog::flush_on(spdlog::level::warn);
}
