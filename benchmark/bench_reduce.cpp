#include <benchmark/benchmark.h>
#include <array>
#include <random>
#include <cstdint>
#include <thread>
#include "math/primes.h"

using namespace math;

static void BM_Reduce_N1_E212_RNS_Base(benchmark::State& state) {
    alignas(64) std::array<uint64_t, 1> p = {12287ULL};
    uint64_t alpha = 3ULL;
    uint32_t e2 = 12;

    MontgomeryReducer<1> reducer(p, alpha, e2);
    
    alignas(64) std::array<uint64_t, 2> a = {0xFEDCBA0987654321ULL, 0x1234567890ABCDEFULL};
    alignas(64) std::array<uint64_t, 1> r = {0};

    for (auto _ : state) {
        reducer.reduce(r, a);
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Reduce_N1_E212_RNS_Base)->ThreadRange(1, std::thread::hardware_concurrency())->UseRealTime();

static void BM_Reduce_N2_E232_DoubleWord(benchmark::State& state) {
    alignas(64) std::array<uint64_t, 2> p = {0xFFFFFFFFFFFFFFFFULL, 0x5555555555555555ULL};
    uint64_t alpha = 0x5555555555555555ULL;
    uint32_t e2 = 32;

    MontgomeryReducer<2> reducer(p, alpha, e2);
    
    alignas(64) std::array<uint64_t, 4> a = {0x1ULL, 0x2ULL, 0x3ULL, 0x4ULL};
    alignas(64) std::array<uint64_t, 2> r = {0, 0};

    for (auto _ : state) {
        reducer.reduce(r, a);
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Reduce_N2_E232_DoubleWord)->ThreadRange(1, std::thread::hardware_concurrency())->UseRealTime();

static void BM_Reduce_N4_E212_Kyber_Like(benchmark::State& state) {
    alignas(64) std::array<uint64_t, 4> p = {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 
                                             0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL};
    uint64_t alpha = 0xAAAAAAAAAAAAAAAAULL;
    uint32_t e2 = 12; 

    MontgomeryReducer<4> reducer(p, alpha, e2);
    
    alignas(64) std::array<uint64_t, 8> a;
    alignas(64) std::array<uint64_t, 4> r;
    std::mt19937_64 rng(12345 + state.thread_index());
    for(auto& x : a) x = rng();

    for (auto _ : state) {
        reducer.reduce(r, a);
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Reduce_N4_E212_Kyber_Like)->ThreadRange(1, std::thread::hardware_concurrency())->UseRealTime();

static void BM_Reduce_N8_E264_MidSize(benchmark::State& state) {
    alignas(64) std::array<uint64_t, 8> p;
    p.fill(0xFFFFFFFFFFFFFFFFULL);
    p[7] = 0x7FFFFFFFFFFFFFFFULL;
    
    uint64_t alpha = 0x9999999999999999ULL;
    uint32_t e2 = 64;

    MontgomeryReducer<8> reducer(p, alpha, e2);
    
    alignas(64) std::array<uint64_t, 16> a;
    alignas(64) std::array<uint64_t, 8> r;
    std::mt19937_64 rng(12345 + state.thread_index());
    for(auto& x : a) x = rng();

    for (auto _ : state) {
        reducer.reduce(r, a);
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Reduce_N8_E264_MidSize)->ThreadRange(1, std::thread::hardware_concurrency())->UseRealTime();

static void BM_Reduce_N32_E212_LargeFHE(benchmark::State& state) {
    alignas(64) std::array<uint64_t, 32> p;
    p.fill(0xFFFFFFFFFFFFFFFFULL);
    p[31] = 0x7FFFFFFFFFFFFFFFULL;
    
    uint64_t alpha = 0x5555555555555555ULL;
    uint32_t e2 = 12;

    MontgomeryReducer<32> reducer(p, alpha, e2);
    
    alignas(64) std::array<uint64_t, 64> a;
    alignas(64) std::array<uint64_t, 32> r;
    std::mt19937_64 rng(12345 + state.thread_index());
    for(auto& x : a) x = rng();

    for (auto _ : state) {
        reducer.reduce(r, a);
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Reduce_N32_E212_LargeFHE)->ThreadRange(1, std::thread::hardware_concurrency())->UseRealTime();

static void BM_Reduce_N64_E2127_Stress(benchmark::State& state) {
    alignas(64) std::array<uint64_t, 64> p;
    p.fill(0xFFFFFFFFFFFFFFFFULL);
    p[63] = 0x7FFFFFFFFFFFFFFFULL;
    
    uint64_t alpha = 0xAAAAAAAAAAAAAAAAULL;
    uint32_t e2 = 127; 

    MontgomeryReducer<64> reducer(p, alpha, e2);
    
    alignas(64) std::array<uint64_t, 128> a;
    a.fill(0xFFFFFFFFFFFFFFFFULL);
    a[127] = 0x3FFFFFFFFFFFFFFFULL;
    
    alignas(64) std::array<uint64_t, 64> r;

    for (auto _ : state) {
        reducer.reduce(r, a);
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Reduce_N64_E2127_Stress)->ThreadRange(1, std::thread::hardware_concurrency())->UseRealTime();

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
