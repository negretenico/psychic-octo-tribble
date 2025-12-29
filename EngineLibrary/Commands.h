#pragma once
#include <cstdint>
#include <variant>
namespace Commands {
	enum class CommandType : uint8_t {
		NewOrder = 1,
	};
	enum class Side : uint8_t {
		Buy = 0,
		Sell = 1,
	};
	struct NewOrder {
		static constexpr const CommandType type = CommandType::NewOrder;
		uint64_t order_id;
		uint64_t price;
		uint32_t qty;
		Side  side;
	};
	using EngineCommands = std::variant<NewOrder>;
}