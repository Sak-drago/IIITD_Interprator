/**
 * @file asserts.hpp
 * @author Just Somebody
 * @date 09-01-2026
 * @brief Asserts and TODOs
 * @note modified from Forge Library (https://github.com/Asher-Ul-Haque/ForgeLibrary)
*/
#pragma once
#include <utils/logger.hpp>


// - - - Asserts - - - 

/// @brief Compile-time assert, use for checking conditions and invariants at compile time.
#if defined(__clang__) || defined(__gcc__)
  #define COMPILE_TIME_ASSERT _Static_assert
#else
  #define COMPILE_TIME_ASSERT static_assert
#endif


#define WHERE_STR __FILE__ << ":" << __LINE__ << " (" << __func__ << ")"


// - - - Runtime asserts - - - 

/// @brief Checks the given expression at runtime, if it evaluates to false, logs a fatal error and aborts the program.
#define RUNTIME_ASSERT(EXPRESSION)                                      \
  if (!(EXPRESSION))                                                    \
  {                                                                     \
    LOG_FATAL("ASSERT FAILED: " #EXPRESSION " @ " << WHERE_STR);        \
    std::abort();                                                       \
  }

/// @brief Checks the given expression at runtime, if it evaluates to false, logs a fatal error with a custom message and aborts the program.
#define RUNTIME_ASSERT_MESSAGE(EXPRESSION, MESSAGE)                     \
  if (!(EXPRESSION))                                                    \
  {                                                                     \
    LOG_FATAL("ASSERT FAILED: " #EXPRESSION                             \
              << " | " << MESSAGE                                       \
              << " @ " << WHERE_STR);                                   \
    std::abort();                                                       \
  }


// - - - TODOs - - -

/// @brief Marks a section of code as TODO, logs an error message and exits the program when reached.
#define TODO                                                            \
  {                                                                     \
    LOG_ERROR("TODO reached @ " << WHERE_STR);                          \
    std::exit(EXIT_FAILURE);                                            \
  }


/// @brief Marks a section of code as TODO with a custom message, logs an error message and exits the program when reached.
#define TODO_COMMENT(MESSAGE)                                           \
  {                                                                     \
    LOG_ERROR("TODO: " << MESSAGE << " @ " << WHERE_STR);               \
    std::exit(EXIT_FAILURE);                                            \
  }
