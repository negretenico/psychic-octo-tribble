#pragma once
#include "Commands.h"
#include "FileUtility.h"
#include <vector>
namespace Utility {
	using DecodedCommands = std::vector<Commands::EngineCommands>;
	DecodedCommands DecodeNewOrderCommand(const Utility::OrderFileData& data);
}

