#pragma once
#include "Commands.h"
#include "Events.h"
#include "OrderBook.h"
namespace Engine {
	class MatchingEngine
	{
	public:
		Events::Event apply(const Commands::EngineCommands& command);
	private:
		Orders::OrderBook order_book;
	};
}
