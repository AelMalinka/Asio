/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{
	namespace Tethys = Entropy::Tethys;

	TEST_CLASS(Task) {
		TEST_METHOD(Create) {
			Tethys::Task task_empty;
			Tethys::Task task([](){});
		}

		TEST_METHOD(Call) {
			bool fail = true;
			Tethys::Task t([&fail]() {
				fail = false;
			});

			Assert::IsTrue(fail);
			t();
			Assert::IsFalse(fail);
		}
	};

	TEST_CLASS(Application) {
		TEST_METHOD(Create) {
			Tethys::Application app;

			app();
		}

		TEST_METHOD(Run) {
			Tethys::Application app;
			bool fail = true;
			Tethys::Task t([&fail]() {
				fail = false;
			});

			app.Add(t);
			Assert::IsTrue(fail);
			app();
			Assert::IsFalse(fail);
		}
	};
}
