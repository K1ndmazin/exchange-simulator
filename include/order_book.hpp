#pragma once
#include <map>
#include <list>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <memory_resource>

enum class OrderStatus{ Pending, Shipped, Delivered };

enum class Side { Buy, Sell };

struct Order{
 int id;
 int quantity;
 double price;
 Side side;
 OrderStatus status;
};

using OrderList = std::pmr::list<Order>; 

struct OrderLocation {
        Side side;
        double price;
        OrderList::iterator handle;
};

using AskMap = std::pmr::map<double, OrderList>;
using BidMap = std::pmr::map<double, OrderList, std::greater<double>>;
using OrderIndex = std::unordered_map<int, OrderLocation>; 

class LimitOrderBook {
public:
    explicit LimitOrderBook(std::pmr::memory_resource* mem_resource = std::pmr::get_default_resource())
        : bids(mem_resource), asks(mem_resource) {}

void add_order(Order order) {
    if (order.side == Side::Buy) {
        OrderList& level = bids[order.price];
        level.push_back(order);
        auto it = std::prev(level.end());
        order_index[order.id] = OrderLocation{order.side, order.price, it};
    } else {
        OrderList& level = asks[order.price];
        level.push_back(order);
        auto it = std::prev(level.end());
        order_index[order.id] = OrderLocation{order.side, order.price, it};
    }
}
void cancel(int id) {
    auto location = order_index.find(id);
    if (location != order_index.end()) {
        auto order_struct = location->second;
        if (order_struct.side == Side::Buy) {
            bids[order_struct.price].erase(order_struct.handle);
            if (bids[order_struct.price].empty()) {
                bids.erase(order_struct.price);
            }
        } else {
            asks[order_struct.price].erase(order_struct.handle);
            if (asks[order_struct.price].empty()) {
                asks.erase(order_struct.price);
            }
        }
        order_index.erase(id);
    }
}
bool asks_empty() const {
    return asks.empty();
}
bool bids_empty() const {
    return bids.empty();
}
Order& get_best_ask(){
        return asks.begin()->second.front();
    }
Order& get_best_bid(){
        return bids.begin()->second.front();
    }
void pop_best_ask(){
   auto  best_ask = asks.begin();
        best_ask->second.pop_front();
        if (best_ask->second.empty()){
                asks.erase(best_ask);
               }
}
void pop_best_bid(){
   auto best_bid = bids.begin();
        best_bid->second.pop_front();
        if (best_bid->second.empty()){
                bids.erase(best_bid);
                }
}
void print_asks_at(double price) {
    for (Order& order : asks[price]){
                std::cout << "id:" << order.id << " qty:" << order.quantity << " | ";
}
}
bool contains(int id) const {
	return order_index.find(id) != order_index.end();
}
private:
    BidMap bids;
    AskMap asks;
    OrderIndex order_index;
};

void match(LimitOrderBook& book, Order incoming_order) {
    if (incoming_order.side == Side::Buy) {
        while (incoming_order.quantity > 0 && !book.asks_empty() && incoming_order.price >= book.get_best_ask().price) {
            auto& resting = book.get_best_ask();
            if (incoming_order.quantity >= resting.quantity) {
                incoming_order.quantity -= resting.quantity;
                book.pop_best_ask();
            } else {
                resting.quantity -= incoming_order.quantity;
                incoming_order.quantity = 0;
            }
        }
    } else {
        while (incoming_order.quantity > 0 && !book.bids_empty() && incoming_order.price <= book.get_best_bid().price) {
            auto& resting = book.get_best_bid();
            if (incoming_order.quantity >= resting.quantity) {
                incoming_order.quantity -= resting.quantity;
                book.pop_best_bid();
            } else {
                resting.quantity -= incoming_order.quantity;
                incoming_order.quantity = 0;
            }
        }
    }

    if (incoming_order.quantity > 0) {
        book.add_order(incoming_order);
    }
}
void process_orders(std::vector<Order>& orders){
        for (Order& order : orders){
                if (order.status == OrderStatus::Pending){
                        order.price *= 1.05;
                        order.status = OrderStatus::Shipped;
                }
        }
}
