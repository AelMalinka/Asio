/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "UV/Timer.hh"
#include <gtest/gtest.h>

using namespace Entropy::Tethys::UV;
using namespace testing;
using namespace std;
using namespace std::chrono;

#define TEST_BEGIN try {
#define TEST_END } catch(exception &e) { FAIL() << e << endl; }

namespace {
	TEST(UVTimer, Create) {
		TEST_BEGIN
			bool success = false;
			Timer timer(milliseconds(100), milliseconds(100), [&success]() {
				success = true;
			});

			EXPECT_FALSE(success);
		TEST_END
	}

	TEST(UVTimer, Run) {
		TEST_BEGIN
			Loop loop;
			bool success = false;
			Timer timer(milliseconds(100), milliseconds(100), [&success, &timer]() {
				success = true;
				timer.Stop();
			});
			loop.Add(timer);

			EXPECT_FALSE(success);
			loop();
			EXPECT_TRUE(success);
		TEST_END
	}
}
