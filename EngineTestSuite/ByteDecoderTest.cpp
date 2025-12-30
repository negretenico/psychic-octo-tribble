#include "pch.h"
#include "../EngineLibrary/ByteDecoder.h"
#include "../EngineLibrary/Commands.h"
#include "../EngineLibrary/FileUtility.h"
#include "CppUnitTest.h"
#include <cstdint>
#include <variant>
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Utility;
namespace UtilityTest
{
	TEST_CLASS(ByteDecoderTest)
	{
	public:
		TEST_METHOD(GivenAnEmptyArrayOfBytesThenIShouldGetBackNoOrders) {
			Utility::DecodedCommands orders = Utility::DecodeNewOrderCommand(Utility::OrderFileData{});
			Assert::IsTrue(orders.empty());
		}
		TEST_METHOD(GivenAnArrayOfBytesthenIshouldGetBackorders) {
			Utility::DecodedCommands orders = Utility::DecodeNewOrderCommand(Utility::OrderFileData{
				0x01,             // msg type
				0x15, 0x00,       // length = 21 (uint16)
				0x01,0,0,0,0,0,0,0, // order_id = 1 (uint64)
				0x01,             // side
				0x50,0,0,0,0,0,0,0, // price = 80 (uint64)
				0x32,0,0,0        // quantity = 50 (uint32)
				});
			Assert::IsFalse(orders.empty());
			Assert::AreEqual(static_cast<size_t>(1), orders.size());
			Commands::NewOrder order = std::get<Commands::NewOrder>(orders[0]);
			Assert::AreEqual(static_cast<uint64_t>(1), order.order_id);
			Assert::AreEqual(static_cast<uint8_t>(Commands::Side::Sell), static_cast<uint8_t>(order.side));
			Assert::AreEqual(static_cast<uint64_t>(80), order.price);
			Assert::AreEqual(static_cast<uint32_t>(50), order.qty);
		}
	};
}
