#include <benchmark/benchmark.h>
#include "order_book.hpp"

static void BM_MatchBuySweep(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        LimitOrderBook book;
        book.add_order({1, 30, 50, Side::Sell, OrderStatus::Pending});
        book.add_order({2, 20, 51, Side::Sell, OrderStatus::Pending});
        Order incoming = {43, 45, 58, Side::Buy, OrderStatus::Pending};
        state.ResumeTiming();

        match(book, incoming);
        benchmark::DoNotOptimize(book);
    }
}
BENCHMARK(BM_MatchBuySweep);


