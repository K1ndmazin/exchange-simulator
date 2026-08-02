#include <benchmark/benchmark.h>
#include <random>
#include "order_book.hpp"
LimitOrderBook build_random_book(int num_orders) {
    LimitOrderBook book;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> price_dist(0, 29);
    std::uniform_int_distribution<int> qty_dist(1, 100);
    std::uniform_int_distribution<int> side_dist(0, 1);

    for (int i = 0; i < num_orders; ++i) {
        int qty = qty_dist(rng);
        double price = price_dist(rng) + 50;
        Side side = (side_dist(rng) == 0) ? Side::Buy : Side::Sell;

        Order order{i, qty, price, side, OrderStatus::Pending};
        book.add_order(order);
    }
    return book;
}
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

static void BM_MatchAgainstLargeBook(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        LimitOrderBook book = build_random_book(1000);
        Order incoming = {1001, 50, 65, Side::Buy, OrderStatus::Pending};
        state.ResumeTiming();

        match(book, incoming);
        benchmark::DoNotOptimize(book);
    }
}
BENCHMARK(BM_MatchAgainstLargeBook);

