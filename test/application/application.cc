/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Application.hh"
#include <gtest/gtest.h>

using namespace Entropy::Tethys;
using namespace testing;
using namespace std;

namespace {
	TEST(Application, Create) {
		Application app;
		app();
	}

	TEST(Application, Run) {
		Application app;
		bool fail = true;
		Task t([&fail]() {
			fail = false;
		});

		EXPECT_TRUE(fail);
		app.Add(t);
		app();
		EXPECT_FALSE(fail);
	}
}
