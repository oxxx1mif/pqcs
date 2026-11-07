#define BOOST_TEST_MODULE PrimesMontgomeryTests
#include <boost/test/unit_test.hpp>
#include <array>
#include <cstdint>
#include <print>

#include "math/primes.h"

using namespace math;

BOOST_AUTO_TEST_CASE(test_montgomery_open_vectors_case1) {
    constexpr size_t N = 64;
    alignas(64) std::array<uint64_t, N> p;
    p.fill(0xFFFFFFFFFFFFFFFFULL);
    p[N - 1] = 0x7FFFFFFFFFFFFFFFULL;

    uint64_t alpha = 0xAAAAAAAAAAAAAAAAULL;
    uint32_t e2 = 12;

    MontgomeryReducer<N> reducer(p, alpha, e2);

    alignas(64) std::array<uint64_t, 2 * N> a;
    a.fill(0xFFFFFFFFFFFFFFFFULL);
    a[2 * N - 1] = 0x3FFFFFFFFFFFFFFFULL;

    alignas(64) std::array<uint64_t, N> r;
    r.fill(0);

    reducer.reduce(r, a);

    uint64_t expected_r0 = 0xdd3bda0e38aaa001ULL;
    uint64_t expected_r63 = 0x4000000000000000ULL;

    std::print("\n[CASE 1] Expected r[0]: 0x{:x}, Actual: 0x{:x}\n", expected_r0, r[0]);
    std::print("[CASE 1] Expected r[63]: 0x{:x}, Actual: 0x{:x}\n", expected_r63, r[N - 1]);

    BOOST_CHECK_EQUAL(r[0], expected_r0);
    BOOST_CHECK_EQUAL(r[N - 1], expected_r63);
}

BOOST_AUTO_TEST_CASE(test_montgomery_open_vectors_case2) {
    constexpr size_t N = 64;
    alignas(64) std::array<uint64_t, N> p;
    p.fill(0xFFFFFFFFFFFFFFFFULL);
    p[N - 1] = 0x7FFFFFFFFFFFFFFFULL;

    uint64_t alpha = 0xAAAAAAAAAAAAAAAAULL;
    uint32_t e2 = 32;

    MontgomeryReducer<N> reducer(p, alpha, e2);

    alignas(64) std::array<uint64_t, 2 * N> a;
    a.fill(0xFFFFFFFFFFFFFFFFULL);
    a[2 * N - 1] = 0x3FFFFFFFFFFFFFFFULL;

    alignas(64) std::array<uint64_t, N> r;
    r.fill(0);

    reducer.reduce(r, a);

    uint64_t expected_r0 = 0xffffffff00000000ULL;
    uint64_t expected_r63 = 0x4000000000000000ULL;

    std::print("[CASE 2] Expected r[0]: 0x{:x}, Actual: 0x{:x}\n", expected_r0, r[0]);
    std::print("[CASE 2] Expected r[63]: 0x{:x}, Actual: 0x{:x}\n", expected_r63, r[N - 1]);

    BOOST_CHECK_EQUAL(r[0], expected_r0);
    BOOST_CHECK_EQUAL(r[N - 1], expected_r63);
}

BOOST_AUTO_TEST_CASE(test_montgomery_open_vectors_case3) {
    constexpr size_t N = 64;
    alignas(64) std::array<uint64_t, N> p;
    p.fill(0x5555555555555555ULL);
    p[N - 1] = 0x3555555555555555ULL;

    uint64_t alpha = 0x1122334455667788ULL;
    uint32_t e2 = 12;

    MontgomeryReducer<N> reducer(p, alpha, e2);

    alignas(64) std::array<uint64_t, 2 * N> a;
    a.fill(0x1111111111111111ULL);

    alignas(64) std::array<uint64_t, N> r;
    r.fill(0);

    reducer.reduce(r, a);

    uint64_t expected_r0 = 0x20cd61beee9a6112ULL;

    std::print("[CASE 3] Expected r[0]: 0x{:x}, Actual: 0x{:x}\n", expected_r0, r[0]);

    BOOST_CHECK_EQUAL(r[0], expected_r0);
}

BOOST_AUTO_TEST_CASE(test_montgomery_open_vectors_case4) {
    constexpr size_t N = 4;
    std::array<uint64_t, N> p = {
        0xFFFFFFFFFFFFFFFFULL, 
        0xFFFFFFFFFFFFFFFFULL, 
        0xFFFFFFFFFFFFFFFFULL, 
        0x7FFFFFFFFFFFFFFFULL
    };
    uint64_t alpha = 0xAAAAAAAAAAAAAAAAULL; 
    uint32_t e2 = 12; 

    MontgomeryReducer<N> reducer(p, alpha, e2);

    std::array<uint64_t, 2 * N> a = {
        0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
        0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
        0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
        0xFFFFFFFFFFFFFFFFULL, 0x3FFFFFFFFFFFFFFFULL
    };

    std::array<uint64_t, N> r = {0};

    reducer.reduce(r, a);

    uint64_t expected_r0 = 0x4f02aaa38dfff001ULL;
    uint64_t expected_r3 = 0x4000000000000000ULL;

    std::print("[CASE 4] Expected r[0]: 0x{:x}, Actual: 0x{:x}\n", expected_r0, r[0]);
    std::print("[CASE 4] Expected r[3]: 0x{:x}, Actual: 0x{:x}\n", expected_r3, r[N - 1]);

    BOOST_CHECK_EQUAL(r[0], expected_r0);
    BOOST_CHECK_EQUAL(r[N - 1], expected_r3);
}
