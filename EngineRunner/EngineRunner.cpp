#include "../EngineLibrary/ByteDecoder.h"
#include "../EngineLibrary/Events.h"
#include "../EngineLibrary/FileUtility.h"
#include "../EngineLibrary/MatchingEngine.h"
#include <algorithm>
#include <cstdio> 
#include <iostream>
#include <optional> 
#include <ranges>
#include <string_view> 
auto for_each_func = [](const Events::Event& event) {
	switch (event) {
	case Events::Event::Accepted:
		std::printf("Order Accepted\n");
		break;
	case Events::Event::Rejected:
		std::printf("Order Rejected\n");
		break;
	default:
		std::printf("Unknown Event\n");
		break;
	}
	};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: EngineRunner <orders.bin>\n";
		return 1;
	}
	std::string_view file_path(argv[1]);
	std::optional<Utility::OrderFileData> orderData = Utility::ReadOrderFile(file_path);
	if (!orderData.has_value()) {
		std::printf("Failed to read order file from path: %s\n", file_path.data());
		return 1;
	}
	auto orders = Utility::DecodeNewOrderCommand(orderData.value());
	Engine::MatchingEngine engine;
	std::ranges::for_each(orders |
		std::views::transform([&engine](const auto& command) {
			return engine.apply(command);
			})
		, for_each_func);
	return 0;
}