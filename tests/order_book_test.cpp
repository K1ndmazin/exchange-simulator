#include <gtest/gtest.h>
#include "order_book.hpp"

TEST(MatchTest, BuyOrderSweepsMultiplePriceLevels) {
    LimitOrderBook book;

    Order order_1 = {1, 30, 50, Side::Sell, OrderStatus::Pending};
    Order order_2 = {2, 20, 51, Side::Sell, OrderStatus::Pending};
    Order incoming_order = {43, 45, 58, Side::Buy, OrderStatus::Pending};

    book.add_order(order_1);
    book.add_order(order_2);
    match(book, incoming_order);

    EXPECT_EQ(book.get_best_ask().price, 51);
    EXPECT_EQ(book.get_best_ask().quantity, 5);
}

TEST(CancelTest, CancelRemovesCorrectOrderFromMiddleOfList) {
    LimitOrderBook book;

    Order order_3 = {3, 15, 52, Side::Sell, OrderStatus::Pending};
    Order order_4 = {4, 25, 52, Side::Sell, OrderStatus::Pending};

    book.add_order(order_3);
    book.add_order(order_4);
    book.cancel(3);

    EXPECT_FALSE(book.contains(3));
    EXPECT_TRUE(book.contains(4));
}
