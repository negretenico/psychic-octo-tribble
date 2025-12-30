#pragma once
#include "Orders.h"
#include <cstdint> 
#include <map>
using OrderId = uint64_t;

namespace Orders {
	class OrderBook {
	public:
		bool add_order(OrderId id, Orders::Order data);
		bool order_exists(OrderId id);

	private:
		std::map<OrderId, Orders::Order> order_book;
	};
}