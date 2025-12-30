#include "pch.h"
#include "Commands.h"
#include "Events.h"
#include "MatchingEngine.h"
#include "Orders.h"
#include <variant>
namespace Engine {
	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

	Events::Event MatchingEngine::apply(const Commands::EngineCommands& command) {
		return std::visit(overload{
				[this](const Commands::NewOrder& command) {
				if (order_book.order_exists(command.order_id)) {
					return Events::Event::Rejected;
				}
				Orders::Order order = Orders::Order::Accepted; // for now accept all orders
					bool added = order_book.add_order(command.order_id, order);
					return added ? Events::Event::Accepted : Events::Event::Rejected;
				},
				[](auto&&) {
					return Events::Event::Unknown;
				}
			}, command);
	}
}