#pragma once
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace Utility {
	using OrderFileData = std::vector<std::uint8_t>;
	std::optional<OrderFileData> ReadOrderFile(std::string_view file_path);
}