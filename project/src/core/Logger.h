#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "../src/resources/Config.h"

class Logger {
public:
  enum LogLevel {
    INFO,
    WARN,
    ERROR
  };

  static const char* logLevelToString(LogLevel level) {
    switch (level) {
      case INFO: return "INFO";
      case WARN: return "WARN";
      case ERROR: return "ERROR";
      default: return "UNKNOWN";
    }
  }

  static void log(LogLevel level, const char* message) {
    if (!LOG_ENABLED) return;

    Serial.print("[");
    Serial.print(logLevelToString(level));
    Serial.print("] ");
    Serial.println(message);
  }

  static void info(const char* message) {
    log(LogLevel::INFO, message);
  }

  static void warn(const char* message) {
    log(LogLevel::WARN, message);
  }

  static void error(const char* message) {
    log(LogLevel::ERROR, message);
  }
};

#endif
