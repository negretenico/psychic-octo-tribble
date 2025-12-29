#include "../EngineLibrary/ByteDecoder.h"
#include "../EngineLibrary/Commands.h"
#include "../EngineLibrary/FileUtility.h"
#include <algorithm>
#include <cstdio> 
#include <iostream>
#include <optional> 
#include <string_view> 
#include <type_traits>
#include <variant>
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
	std::for_each(orders.begin(), orders.end(), [](const Commands::EngineCommands& commands) {
		std::visit([](const auto& command) {
			using T = std::decay_t<decltype(command)>;
			if constexpr (std::is_same_v<T, Commands::NewOrder>) {
				std::printf("NewOrder - OrderID: %llu, Price: %llu, Qty: %u, Side: %u\n",
					command.order_id, command.price, command.qty, command.side);
			}
			}, commands);
		});
	return 0;
}