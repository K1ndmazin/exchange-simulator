#include <vector>
#include <map>
#include <deque>

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

class LimitOrderBook {
public:
    void add_order(Order order){
        if (order.side == Side::Buy){
                bids[order.price].push_back(order);
        }
        if (order.side == Side:: Sell){
                asks[order.price].push_back(order);
        }
}
private:
    std::map<double, std::deque<Order>> bids;
    std::map<double, std::deque<Order>> asks;
};


void process_orders(std::vector<Order>& orders){
        for (Order& order : orders){
                if (order.status == OrderStatus::Pending){
                        order.price *= 1.05;
                        order.status = OrderStatus::Shipped;
                }
        }
}
