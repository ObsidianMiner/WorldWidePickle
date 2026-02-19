#include "pch.h"
#include "CppUnitTest.h"
#include "Browser/Browser.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WWPUnitTests
{
	TEST_CLASS(Browser)
	{
	public:
		
		TEST_METHOD(Bootup)
		{
			Assert::IsTrue(true);
		}
	};
}
