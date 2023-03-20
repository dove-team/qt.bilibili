#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
using namespace std;

class logger
{
private :
	shared_ptr<spdlog::logger> log;
	auto init() {
		if (log) {
			log = spdlog::daily_logger_mt("bilibili_log", "logs/client.txt", 2);
			log->flush_on(spdlog::level::info);
			spdlog::flush_every(std::chrono::seconds(10));
			spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%l] [%t] - <%s>|<%#>|<%!>,%v");
		}
		return log;
	}
public:
	void logInfo(string info) {
		auto log = init();
		spdlog::info(info);
	}
	void logError(string info) {
		auto log = init();
		spdlog::error(info);
	}
	void logError(string info, exception ex) {
		auto log = init();
		auto msg = ex.what();
		spdlog::error(info);
		spdlog::error(msg);
	}
};