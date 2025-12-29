#include "pch.h"
#include "ByteDecoder.h"
#include "Commands.h"
#include "FileUtility.h"
#include <cstdint>
#include <string.h>
#include <type_traits>
#include <utility>
#include <vector>
namespace Utility {
	template<typename T>
	using OffestDecodedValuePair = std::pair<size_t, T>;

	template<typename T>
	OffestDecodedValuePair<T> DecodeBytes(const std::vector<uint8_t>& bytes, const size_t& current_offset) {
		static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
		T value;
		std::memcpy(&value, bytes.data() + current_offset, sizeof(T));
		size_t offset = sizeof(T);
		return std::make_pair(offset + current_offset, value);
	}
	DecodedCommands DecodeNewOrderCommand(const Utility::OrderFileData& data) {
		size_t offest = 0;
		DecodedCommands decoded_orders;
		while (offest < data.size()) {
			// first byte is the header 
			OffestDecodedValuePair<uint8_t> header = DecodeBytes<uint8_t>(data, offest);
			offest = header.first;
			// second byte is the lenght 
			OffestDecodedValuePair<uint16_t> len = DecodeBytes<uint16_t>(data, offest);
			offest = len.first;
			if (header.second != 1) {
				offest += len.second; // skip n bytes
				continue;
			}
			// remaing n bytes are the payload. 
			// we need to read the header check for correctness and then read the nlenght and then read the payload.
			OffestDecodedValuePair<uint64_t> order_id = DecodeBytes<uint64_t>(data, offest);
			offest = order_id.first;
			OffestDecodedValuePair<uint8_t> side = DecodeBytes<uint8_t>(data, offest);
			offest = side.first;
			OffestDecodedValuePair<uint64_t> price = DecodeBytes<uint64_t>(data, offest);
			offest = price.first;
			OffestDecodedValuePair<uint32_t> quantity = DecodeBytes<uint32_t>(data, offest);
			offest = quantity.first;
			decoded_orders.push_back(Commands::NewOrder{
				.order_id = order_id.second,
				.price = price.second,
				.qty = quantity.second,
				.side = side.second == 0 ? Commands::Side::Buy : Commands::Side::Sell,
				});
		}
		return decoded_orders;
	}

}