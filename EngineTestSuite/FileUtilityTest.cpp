#include "pch.h"
#include "../EngineLibrary/FileUtility.h"
#include "CppUnitTest.h"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string_view>
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Utility;
namespace UtilityTest
{
	TEST_CLASS(FileUtilityTest)
	{
	public:
		TEST_METHOD(GivenASuccessfulLoadAndReadOfFileContentThenIShouldGetBackTheRawFileContents)
		{
			namespace fs = std::filesystem;
			fs::path temp = fs::temp_directory_path() / "orders_test.bin";
			std::vector<std::uint16_t> expected{
				1, 42, 1337, 65535
			};
			{
				std::ofstream out(temp, std::ios::binary);
				Assert::IsTrue(out.good());

				out.write(reinterpret_cast<const char*>(expected.data()),
					expected.size() * sizeof(std::uint16_t));
			}
			auto fileContent = ReadOrderFile(temp.string());
			Assert::IsTrue(fileContent.has_value());
			fs::remove(temp);
		}
		TEST_METHOD(GivenAnImproperFilePathThenIShouldGetNothingBack)
		{
			std::string_view file_name = "";
			auto fileContent = ReadOrderFile(file_name);
			Assert::IsFalse(fileContent.has_value());
		}
		TEST_METHOD(GivenAnIssueReadingTheFileContentThenIGetNothingBack)
		{
			std::string_view file_name = "";
			auto fileContent = ReadOrderFile(file_name);
			Assert::IsFalse(fileContent.has_value());
		}
	};
}
