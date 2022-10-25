#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <time.h>

LogEntry logEntry;
std::vector<LogEntry> Logger::messages;

std::string CurrentDataTimeToString() {
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%H:%S", localtime(&time));
	return output;
}

void Logger::Log(const std::string& message) {
	logEntry.type = LOG_INFO;
	logEntry.message = "LOG: [" + CurrentDataTimeToString() + "]: " + message;
	std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
	logEntry.type = LOG_ERROR;
	logEntry.message = "ERR: [" + CurrentDataTimeToString() + "]: " + message;
	std::cout << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
}