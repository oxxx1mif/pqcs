//
// Copyright 2026 Gleb Obitotsky
//
// SPDX-License-Identifier: Apache-2.0
//
// Implementation of Montgomery word-wise reduction for special primes
// of the form p = 2^{e_2} * alpha - 1.
// Based on the paper: J.-C. Bajard, S. Duquesne,
// "Montgomery-friendly primes and applications to cryptography" (2020).
//

#pragma once

#include <array>
#include <cstdint>
#include <new>
#include <span>

namespace math {

/// @brief Montgomery reducer for Montgomery-friendly prime numbers.
/// @tparam N The number of 64-bit words required to represent the prime number
/// p.
template <size_t N>
class alignas(std::hardware_constructive_interference_size) MontgomeryReducer {
 private:
  std::array<uint64_t, N> p_;
  uint64_t alpha_;
  size_t word_shift_;
  uint32_t e2_;
  uint32_t bit_shift_;

 public:
  [[nodiscard]] explicit constexpr MontgomeryReducer(
      const std::array<uint64_t, N>& p, uint64_t alpha, uint32_t e2)
      : p_(p),
        alpha_(alpha),
        word_shift_((e2 >= 64) ? (e2 - 64) / 64 : 0),
        e2_(e2),
        bit_shift_((e2 >= 64) ? (e2 - 64) % 64 : 0) {}

  /// @brief Montgomery word-wise reduction.
  /// @param r Output buffer for the result, size exactly N words (0 <= r < p).
  /// @param a Input number for reduction, size exactly 2 * N words (0 <= a < p
  /// * beta^n).
  void reduce(std::span<uint64_t, N> r,
             std::span<const uint64_t, 2 * N> a) const {}
};

}  // namespace math
