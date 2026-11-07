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

#include <algorithm>
#include <array>
#include <cstdint>
#include <new>
#include <span>

#include "hwy/highway.h"

namespace math {

namespace hn = hwy::HWY_NAMESPACE;

template <size_t N>
class alignas(std::hardware_constructive_interference_size) MontgomeryReducer {
  static_assert(N > 0, "MontgomeryReducer: N must be greater than 0");

 private:
  alignas(64) std::array<uint64_t, N> p_;
  uint64_t alpha_;
  size_t word_shift_;
  uint32_t e2_;
  uint32_t bit_shift_;

  template <size_t WShift, uint32_t BShift>
  [[gnu::always_inline]] inline void reduce_impl(
      std::span<uint64_t, N> r,
      std::span<const uint64_t, 2 * N> a) const noexcept {
    alignas(64) std::array<uint64_t, 2 * N> temp;
    std::copy(a.begin(), a.end(), temp.begin());

    static constexpr uint32_t safe_rev_shift = (64 - BShift) & 63;
    static constexpr uint64_t shift_mask = (BShift == 0) ? 0ULL : ~0ULL;

    for (size_t i = 0; i < N; ++i) {
      uint64_t r0 = temp[i];

      unsigned __int128 prod = static_cast<unsigned __int128>(r0) *
                               static_cast<unsigned __int128>(alpha_);
      uint64_t mul_lo = static_cast<uint64_t>(prod);
      uint64_t mul_hi = static_cast<uint64_t>(prod >> 64);

      uint64_t term_low, term_high;
      if constexpr (BShift == 0) {
        term_low = mul_lo;
        term_high = mul_hi;
      } else {
        term_low = (mul_lo << BShift) | (mul_hi >> safe_rev_shift);
        term_high = (mul_hi << BShift) & shift_mask;
      }

      size_t idx_low = i + WShift;
      size_t idx_high = idx_low + 1;

      if (idx_low < 2 * N) {
        uint64_t old_low = temp[idx_low];
        uint64_t sum_low = old_low + term_low;
        uint64_t carry1 = (sum_low < old_low) ? 1ULL : 0ULL;
        temp[idx_low] = sum_low;

        if (idx_high < 2 * N) {
          uint64_t old_high = temp[idx_high];
          uint64_t partial_sum = old_high + term_high;
          uint64_t carry2 = (partial_sum < old_high) ? 1ULL : 0ULL;

          uint64_t final_sum = partial_sum + carry1;
          uint64_t carry3 = (final_sum < partial_sum) ? 1ULL : 0ULL;

          temp[idx_high] = final_sum;
          uint64_t current_carry = carry2 | carry3;

#pragma clang loop unroll(full)
          for (size_t k = i + WShift + 2; k < 2 * N; ++k) {
            uint64_t old_val = temp[k];
            uint64_t next_sum = old_val + current_carry;
            current_carry = (next_sum < old_val) ? 1ULL : 0ULL;
            temp[k] = next_sum;
          }
        }
      }
    }

    const hn::ScalableTag<uint64_t> d;
    const size_t lanes = hn::Lanes(d);
    const auto v_p = hn::LoadU(d, p_.data());

    for (size_t j = 0; j < N; j += lanes) {
      auto v_r = hn::LoadU(d, &temp[j + N]);
      auto v_diff = hn::Sub(v_r, v_p);
      auto mask_lt = hn::Lt(v_r, v_p);
      auto v_res = hn::IfThenElse(mask_lt, v_r, v_diff);
      hn::StoreU(v_res, d, &r[j]);
    }
  }

 public:
  [[nodiscard]] explicit constexpr MontgomeryReducer(
      const std::array<uint64_t, N>& p, uint64_t alpha, uint32_t e2)
      : p_(p),
        alpha_(alpha),
        word_shift_(e2 / 64),
        bit_shift_(e2 % 64),
        e2_(e2) {}

  [[nodiscard]] constexpr std::array<uint64_t, N> get_modulus() const noexcept {
    return p_;
  }

  void reduce(std::span<uint64_t, N> r,
              std::span<const uint64_t, 2 * N> a) const noexcept {
    if constexpr (N == 1) {
      reduce_impl<0, 0>(r, a);
    } else {
      if (bit_shift_ == 0)
        reduce_impl<0, 0>(r, a);
      else if (bit_shift_ == 12)
        reduce_impl<0, 12>(r, a);
      else if (bit_shift_ == 32)
        reduce_impl<0, 32>(r, a);
      else
        reduce_impl<0, 0>(r, a);
    }
  }
};

}  // namespace math
