#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>
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
RC_GTEST_PROP(MatchProperties, QuantityIsConserved, ()) {
    int qty1 = *rc::gen::inRange(1, 10000);
    int qty2 = *rc::gen::inRange(1, 10000);
    int initial= qty1+qty2;
  
   LimitOrderBook book{};
   Order order{1, qty2, 50, Side::Sell, OrderStatus :: Pending};
   Order incoming_order{2, qty1, 60, Side::Buy, OrderStatus :: Pending};
   book.add_order(order);
   match(book, incoming_order);
if (qty1 > qty2){
 RC_ASSERT(book.asks_empty());
RC_ASSERT(!book.bids_empty());
RC_ASSERT(book.get_best_bid().quantity==qty1-qty2);
} else if (qty2> qty1){
RC_ASSERT(!book.asks_empty());
RC_ASSERT(book.get_best_ask().quantity == qty2- qty1);
}  else {
    RC_ASSERT(book.asks_empty());
    RC_ASSERT(book.bids_empty());
}
}
