#include <vector>
#include <map>
#include <deque>
#include <functional>

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
bool asks_empty() const {
    return asks.empty();
}
bool bids_empty() const {
    return bids.empty();
}
Order& get_best_ask(){
        return asks.begin()->second.front();
    }
void pop_best_ask(){
   auto  best_ask = asks.begin();
        best_ask->second.pop_front();
        if (best_ask->second.empty()){
                asks.erase(best_ask);
               }
}

private:
    std::map<double, std::deque<Order>, std::greater<double>> bids;
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
int main(){
	return 0;
}
