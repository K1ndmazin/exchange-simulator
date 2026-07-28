#include <vector>
#include <map>
#include <list>
#include <deque>
#include <functional>
#include <iostream>

enum class OrderStatus{
        Pending,
        Shipped,
        Delivered
};
enum class Side { Buy, Sell };
struct Order{
 int id;
 int quantity;
 double price;
 Side side;
 OrderStatus status;
};

struct OrderLocation {
	Side side;
	double price;
	std::list<Order>::iterator handle;
};

class LimitOrderBook {
public:
void add_order(Order order) {
    if (order.side == Side::Buy) {
        std::list<Order>& level = bids[order.price];
        level.push_back(order);
        auto it = std::prev(level.end());
        order_index[order.id] = OrderLocation{order.side, order.price, it};
    } else {
        std::list<Order>& level = asks[order.price];
        level.push_back(order);
        auto it = std::prev(level.end());
        order_index[order.id] = OrderLocation{order.side, order.price, it};
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
private:
    std::map<double, std::list<Order>, std::greater<double>> bids;
    std::map<double, std::list<Order>> asks;
    std::unordered_map<int, OrderLocation> order_index;
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

int main(){
        Order order_1 = {1, 30, 50, Side::Sell, OrderStatus::Pending};
        Order order_2 = {2, 20, 51, Side::Sell, OrderStatus::Pending};
        Order incoming_order = {43, 45, 58, Side::Buy, OrderStatus::Pending};
        LimitOrderBook book_1{};
        book_1.add_order(order_1);
        book_1.add_order(order_2);
        match(book_1, incoming_order);
        std::cout << book_1.get_best_ask().price << " " << book_1.get_best_ask().quantity;
        return 0;
}
