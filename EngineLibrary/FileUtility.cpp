#include "pch.h"
#include "FileUtility.h"
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string_view>
namespace fs = std::filesystem;
namespace Utility {
	std::optional<OrderFileData> ReadOrderFile(std::string_view file_path) {
		fs::path ppath(file_path);
		if (!fs::exists(ppath) || !fs::is_regular_file(ppath)) {
			return std::nullopt;
		}
		const auto file_size = fs::file_size(ppath);
		if (file_size == 0 || file_size % sizeof(uint8_t) != 0) {
			return std::nullopt;
		}
		auto order_size = fs::file_size(ppath) / sizeof(uint8_t);
		OrderFileData order_data(order_size);
		std::ifstream file_stream(ppath, std::ios::binary);
		if (!file_stream) {
			return std::nullopt;
		}
		file_stream.read(std::bit_cast<char*>(order_data.data()), file_size);
		return order_data;
	}
}