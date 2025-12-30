#include "pch.h"
#include "OrderBook.h"
#include "Orders.h"
namespace Orders {
	bool OrderBook::add_order(OrderId id, Orders::Order data) {
		auto result = order_book.insert({ id,data });
		return result.second;
	}
	bool OrderBook::order_exists(OrderId id) {
		return order_book.find(id) != order_book.end();
	}
}