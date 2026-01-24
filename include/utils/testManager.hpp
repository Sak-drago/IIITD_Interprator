/**
 * @file testManager.hpp
 * @author Just Somebody
 * @date 15-01-2026
 * @brief Simple test harness made with linux in mind (no crash detection, parallelism on windows)
 * @note Modified from Forge Library
 *       https://github.com/Asher-Ul-Haque/ForgeLibrary
 */

#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <utils/dataTypes.hpp>
#include <utils/logger.hpp>

#define TESTING

/// @brief: all tests must output a single u8 as the exit code
typedef u8 (*TEST)();

/// @brief: tests must have a function and a description 
typedef struct TestEntry
{
  TEST              func;
  const std::string description;
} TestEntry; 

/// @brief: all tests inside a group are executed sequentially
typedef std::vector<TestEntry> TestGroup;


// - - - API

/**
 * @brief: register a test
 * @param FUNCTION : the test function, 
 * @param DESCRIPTION: a description of the test 
 * @param GROUP_ID : all tests in the same group are executed sequentially, since it defaults to 0, all tests are executed sequentially
*/
void registerTest(
  TEST                FUNCTION, 
  const std::string&  DESCRIPTION, 
  u8                  GROUP_ID = 0);

/// @brief: run all the tests that were registered
void runTests();


// - - - Useful macros - - - 

#define EXPECT_TO_BE(EXPECTED, ACTUAL)                                   \
  if ((EXPECTED) != (ACTUAL))                                           \
  {                                                                     \
    LOG_ERROR("EXPECT_TO_BE failed: " #EXPECTED " != " #ACTUAL);        \
    return false;                                                       \
  }

#define EXPECT_NOT_TO_BE(NOT_EXPECTED, ACTUAL)                          \
  if ((NOT_EXPECTED) == (ACTUAL))                                       \
  {                                                                     \
    LOG_ERROR("EXPECT_NOT_TO_BE failed");                               \
    return false;                                                       \
  }

#define EXPECT_STRING_TO_BE(EXPECTED, ACTUAL)                           \
  if ((EXPECTED) != (ACTUAL))                                           \
  {                                                                     \
    LOG_ERROR("EXPECT_STRING_TO_BE failed");                            \
    return false;                                                       \
  }

#define EXPECT_C_STRING_TO_BE(EXPECTED, ACTUAL)                         \
  if (std::strcmp((EXPECTED), (ACTUAL)) != 0)                           \
  {                                                                     \
    LOG_ERROR("EXPECT_C_STRING_TO_BE failed");                          \
    return false;                                                       \
  }

#define EXPECT_FLOAT_TO_BE(EXPECTED, ACTUAL, EPS)                       \
  if (std::fabs((EXPECTED) - (ACTUAL)) > (EPS))                         \
  {                                                                     \
    LOG_ERROR("EXPECT_FLOAT_TO_BE failed");                             \
    return false;                                                       \
  }

#define EXPECT_TO_BE_NULL(PTR)                                          \
  if ((PTR) != nullptr)                                                 \
  {                                                                     \
    LOG_ERROR("EXPECT_TO_BE_NULL failed");                              \
    return false;                                                       \
  }

#define EXPECT_TO_BE_NOT_NULL(PTR)                                      \
  if ((PTR) == nullptr)                                                 \
  {                                                                     \
    LOG_ERROR("EXPECT_TO_BE_NOT_NULL failed");                          \
    return false;                                                       \
  }

#define EXPECT_TO_BE_TRUE(EXPR)                                         \
  if (!(EXPR))                                                          \
  {                                                                     \
    LOG_ERROR("EXPECT_TO_BE_TRUE failed");                              \
    return false;                                                       \
  }

#define EXPECT_TO_BE_FALSE(EXPR)                                        \
  if ((EXPR))                                                           \
  {                                                                     \
    LOG_ERROR("EXPECT_TO_BE_FALSE failed");                             \
    return false;                                                       \
  }

#define SKIP_TEST 2
