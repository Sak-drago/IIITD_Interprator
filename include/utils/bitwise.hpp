/**
 * @file bitwise.hpp
 * @author Just Somebody
 * @date 10-01-2026
 * @brief Simple bitwise macros
 * @note Modified from Forge Library
 *       https://github.com/Asher-Ul-Haque/ForgeLibrary
 */

#pragma once

/// @brief Get the n-th bit of a.
#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

/// @brief Set or clear the n-th bit of a.
#define BIT_SET(a, n, on) { if (on) a |= (1 << n); else a &= ~(1 << n);}

/// @brief Check if a is between b and c (inclusive).
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))
