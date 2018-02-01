/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Task.hh"
#include <gtest/gtest.h>

using namespace Entropy::Tethys;
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

	TEST(Task, After) {
		bool fail = true;
		Task t([](){}, [&fail](){
			fail = false;
		});

		EXPECT_TRUE(fail);
		EXPECT_TRUE(t.hasAfter());
		t.After();
		EXPECT_FALSE(fail);
	}
}
