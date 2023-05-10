#include "pch.h"
#include "CppUnitTest.h"
//#include "../../Map_Reduce/Map_Reduce/Map.h"
#include "../../Map_Reduce/Map/Map/Map.h"

/*
 * MapReduceNativeTest contains test cases for the Tokenize Value method
 */


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MapReduceNativeTest
{
	TEST_CLASS(MapReduceNativeTest)
	{

	public:
		
		//TokenizeValueMethod1 checks a generic string with only one word that has a uppercase letter
		TEST_METHOD(TokenizeValueMethod1)
		{
			//Map map();

			//Map mapper;

			// Tokenize a raw data line
			std::vector<std::string> tokens;
			std::string rawDataLine = "This is an example test.";


			//convert the tokens string to an array
			//mapper.TokenizeValue(rawDataLine, tokens);


			std::vector<std::string> expectedTokens = { "this", "is", "an", "example", "test" };

			Assert::AreEqual(tokens.size(), expectedTokens.size());

			for (int i = 0; i < expectedTokens.size(); i++)
			{
				Assert::AreEqual(expectedTokens[i], tokens[i]);
			}
			

		}


		//TokenizeValueMethod2 checks a generic string with punctuation and uppercase word
		TEST_METHOD(TokenizeValueMethod2)
		{
			//Map map("intermediateDir/");


			std::string rawData = "Th'is is a test: LINE .\n";

			std::vector<std::string> Tokens;


			//map.TokenizeValue(rawData, Tokens);

			std::vector<std::string> secondTokens = { "this", "is", "a", "test", "line", "" };

			Assert::AreEqual(Tokens.size(), secondTokens.size());

			for (int i = 0; i < secondTokens.size(); i++)
			{
				Assert::AreEqual(secondTokens[i], Tokens[i]);
			}

		}


		//TokenizeValueMethod2 checks a string with both non-ascii characters and all special characters on the keyboard
		TEST_METHOD(TokenizeValueMethod3)
		{
			//Map map("intermediateDir/");


			std::string rawData = "h£llo WOR$D w!@#$%^&*()_-+=[]{}|.,;':~""`n?><\n";

			std::vector<std::string> Tokens;


			//map.TokenizeValue(rawData, Tokens);

			std::vector<std::string> secondTokens = { "hllo", "word", "wn" };

			Assert::AreEqual(Tokens.size(), secondTokens.size());

			for (int i = 0; i < secondTokens.size(); i++)
			{
				Assert::AreEqual(secondTokens[i], Tokens[i]);
			}

		}




	};
}
