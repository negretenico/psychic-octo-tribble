#pragma once
#include <cstdint>
namespace Orders {
	enum class Order :uint8_t {
		Accepted = 0,
		Rejected = 1
	};
}