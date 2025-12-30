#include "pch.h"
#include "../EngineLibrary/Commands.h"
#include "../EngineLibrary/Events.h"
#include "../EngineLibrary/MatchingEngine.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MatchingEngineTest
{
	TEST_CLASS(MatchingEngineTest)
	{
	public:
		TEST_METHOD(GivenANewOrderCommandThenTheEngineShouldReturnAccepted) {
			Engine::MatchingEngine engine;
			Commands::NewOrder cmd{ 1, 80, 50, Commands::Side::Sell };
			Events::Event result = engine.apply(cmd);
			Assert::IsTrue(result == Events::Event::Accepted);
		}

		TEST_METHOD(GivenANewOrderCommandAndTheOrderIsInvalidThenIShouldGetARejectedEvent) {
			Engine::MatchingEngine engine;
			Commands::NewOrder cmd{ 1, 80, 50, Commands::Side::Sell };
			// first apply should accept
			Events::Event first = engine.apply(cmd);
			Assert::IsTrue(first == Events::Event::Accepted);
			// second apply with same order id should be rejected
			Events::Event second = engine.apply(cmd);
			Assert::IsTrue(second == Events::Event::Rejected);
		}

		TEST_METHOD(GivenAnotherNewOrderCommandThenTheEngineShouldReturnAccepted) {
			Engine::MatchingEngine engine;
			Commands::NewOrder cmd1{ 1, 80, 50, Commands::Side::Sell };
			Commands::NewOrder cmd2{ 2, 100, 10, Commands::Side::Buy };
			Events::Event r1 = engine.apply(cmd1);
			Events::Event r2 = engine.apply(cmd2);
			Assert::IsTrue(r1 == Events::Event::Accepted);
			Assert::IsTrue(r2 == Events::Event::Accepted);
		}
	};
}