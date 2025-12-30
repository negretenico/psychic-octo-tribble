#include "pch.h"
#include "../EngineLibrary/OrderBook.h"
#include "../EngineLibrary/Orders.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UtilityTest
{
	TEST_CLASS(OrderBookTest)
	{
	public:
		TEST_METHOD(GivenOrder1DoesNotExistThenIShouldBeAbleToAddIt) {
			Orders::OrderBook book;
			Assert::IsFalse(book.order_exists(1));
			book.add_order(1, Orders::Order::Accepted);
			Assert::IsTrue(book.order_exists(1));
		}
		TEST_METHOD(GivenOrder1DoesExistThenIShouldNotBeAbleToAddIt) {
			Orders::OrderBook book;
			Assert::IsTrue(book.add_order(1, Orders::Order::Accepted));
			Assert::IsFalse(book.add_order(1, Orders::Order::Accepted));
		}
	};
}
