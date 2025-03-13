#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "../src/resources/Config.h"

class Log {
public:
    enum LogLevel {
        INFO,
        WARN,
        ERROR
    };

    static const char *logLevelToString(LogLevel level) {
        switch (level) {
            case INFO: return "INFO";
            case WARN: return "WARN";
            case ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    template<typename... Args>
    static void log(LogLevel level, const char *message, Args... args) {
        if (!LOG_ENABLED) return;

        String combinedMessage = message;
        using expander = int[];
        (void) expander {0, ((combinedMessage += args), 0)...};

        Serial.print("[");
        Serial.print(logLevelToString(level));
        Serial.print("] ");
        Serial.println(combinedMessage);
    }

    template<typename... Args>
    static void info(const char *message, Args... args) {
        log(INFO, message, args...);
    }

    template<typename... Args>
    static void warn(const char *message, Args... args) {
        log(WARN, message, args...);
    }

    template<typename... Args>
    static void error(const char *message, Args... args) {
        log(ERROR, message, args...);
    }
};

#endif
