/**
 * @file defines.hpp
 * @author Just Somebody
 * @date 09-01-2026
 * @brief Basic file to include in every other header/source file.
 * @note modified from Forge Library (https://github.com/Asher-Ul-Haque/ForgeLibrary)
*/
#pragma once

#include <utils/logger.hpp>
#include <utils/asserts.hpp>
#include <utils/bitwise.hpp>
#include <utils/dataTypes.hpp>
#include <utils/profiler.hpp>

// - - - Ensure all types are of the correct size.
#if defined(__clang__)
  COMPILE_TIME_ASSERT(sizeof(u8)  == 1, "Expected u8 to be 1 byte.");
  COMPILE_TIME_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
  COMPILE_TIME_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
  COMPILE_TIME_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

  COMPILE_TIME_ASSERT(sizeof(i8)  == 1, "Expected i8 to be 1 byte.");
  COMPILE_TIME_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
  COMPILE_TIME_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
  COMPILE_TIME_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

  COMPILE_TIME_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
  COMPILE_TIME_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
#endif
