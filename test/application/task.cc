/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Task.hh"
#include <gtest/gtest.h>

using namespace Entropy::Asio;
using namespace testing;
using namespace std;

namespace {
	TEST(Task, Create) {
		Task task_emtpy;
		Task task([](){});
	}

	TEST(Task, Call) {
		bool fail = true;
		Task t([&fail]() {
			fail = false;
		});

		EXPECT_TRUE(fail);
		t();
		EXPECT_FALSE(fail);
	}
}
