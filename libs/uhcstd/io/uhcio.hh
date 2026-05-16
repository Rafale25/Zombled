#pragma once

#include <stdio.h>
#include <stdlib.h>

/// Leveled logger with colored console output
namespace Log {
    /// Log level values for filtering output
    enum Level {
        UHC_LOG_TRACE,
        UHC_LOG_DEBUG,
        UHC_LOG_INFO,
        UHC_LOG_WARN,
        UHC_LOG_ERROR,
    };

    /// Log a message at TRACE level
    /// @param fmt printf-style format string
    /// @param ... Format arguments
    void trace(const char* fmt, ...);
    /// Log a message at DEBUG level
    /// @param fmt printf-style format string
    /// @param ... Format arguments
    void debug(const char* fmt, ...);
    /// Log a message at INFO level
    /// @param fmt printf-style format string
    /// @param ... Format arguments
    void info (const char* fmt, ...);
    /// Log a message at WARN level
    /// @param fmt printf-style format string
    /// @param ... Format arguments
    void warn (const char* fmt, ...);
    /// Log a message at ERROR level
    /// @param fmt printf-style format string
    /// @param ... Format arguments
    void error(const char* fmt, ...);
    /// Log a raw message without a level prefix
    /// @param fmt printf-style format string
    /// @param ... Format arguments
    void msg  (const char* fmt, ...);
};

/// Simple file reading utilities
namespace File {
    /// The file objects
    struct It {
        FILE* handle = NULL;
        long long size = 0;
    };
    /// Open a file for reading
    /// @param filePath Path to the file
    /// @returns The file
    File::It open(const char* filePath);
    /// Close an open file
    /// @param file The file
    void close(File::It& file);
    /// Read the entire file contents into a heap-allocated string
    /// @param file The file
    /// @returns Null-terminated file contents — caller must free
    char* read(File::It& file);
};
