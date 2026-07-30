#include "order_book.hpp"
int main(){
        Order order_1 = {1, 30, 50, Side::Sell, OrderStatus::Pending};
        Order order_2 = {2, 20, 51, Side::Sell, OrderStatus::Pending};
	Order order_3 = {3, 15, 52, Side::Sell, OrderStatus::Pending};
	Order order_4 = {4, 25, 52, Side::Sell, OrderStatus::Pending};
	LimitOrderBook book_1{};
	book_1.add_order(order_3);
	book_1.add_order(order_4);
	std::cout << "\nBefore cancel: ";
	book_1.print_asks_at(52);
	book_1.cancel(3);
	std::cout << "\nAfter cancel: ";
	book_1.print_asks_at(52);
        Order incoming_order = {43, 45, 58, Side::Buy, OrderStatus::Pending};
        book_1.add_order(order_1);
        book_1.add_order(order_2);
	book_1.add_order(order_3);
        match(book_1, incoming_order);
        std::cout << book_1.get_best_ask().price << " " << book_1.get_best_ask().quantity;
        return 0;	
}

