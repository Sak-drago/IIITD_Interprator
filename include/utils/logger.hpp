/**
 * @file logger.hpp
 * @author Just Somebody
 * @date 10-01-2026
 * @brief Simple colored console logger
 *
 * Logging levels:
 * - FATAL   : Bright red — critical error, program aborts
 * - ERROR   : Red — recoverable error
 * - WARNING : Orange — non-critical warning
 * - INFO    : Green — informational message
 * - DEBUG   : Blue — debug output
 * - TRACE   : Gray — verbose tracing
 *
 * @note Modified from Forge Library
 *       https://github.com/Asher-Ul-Haque/ForgeLibrary
 */

#pragma once
#include <iostream>

#define LOG_COLOR_FATAL   "\033[1;31m"
#define LOG_COLOR_ERROR   "\033[31m"
#define LOG_COLOR_RESET   "\033[0m"
#ifdef DEBUG
  #define LOG_COLOR_WARNING "\033[33m"
  #define LOG_COLOR_INFO    "\033[32m"
  #define LOG_COLOR_DEBUG   "\033[36m"
  #define LOG_COLOR_TRACE   "\033[90m"
#else 
  #define LOG_COLOR_WARNING ""
  #define LOG_COLOR_INFO    ""
  #define LOG_COLOR_DEBUG   ""
  #define LOG_COLOR_TRACE   ""
#endif

#define LOG_FATAL(x)   std::cerr << LOG_COLOR_FATAL   "[FATAL] "   << x << LOG_COLOR_RESET << '\n';
#define LOG_ERROR(x)   std::cerr << LOG_COLOR_ERROR   "[ERROR] "   << x << LOG_COLOR_RESET << '\n';
#ifdef DEBUG
  #define LOG_WARNING(x) std::cerr << LOG_COLOR_WARNING "[WARNING] " << x << LOG_COLOR_RESET << '\n'
  #define LOG_INFO(x)    std::cout << LOG_COLOR_INFO    "[INFO] "    << x << LOG_COLOR_RESET << '\n'
  #define LOG_DEBUG(x)   std::cout << LOG_COLOR_DEBUG   "[DEBUG] "   << x << LOG_COLOR_RESET << '\n'
  #define LOG_TRACE(x)   std::cout << LOG_COLOR_TRACE   "[TRACE] "   << x << LOG_COLOR_RESET << '\n' 
#else
  #define LOG_WARNING(x)
  #define LOG_INFO(x)
  #define LOG_DEBUG(x)
  #define LOG_TRACE(x)
#endif

