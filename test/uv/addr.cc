/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "UV/GetAddrInfo.hh"
#include <gtest/gtest.h>

using namespace Entropy::Tethys::UV;
using namespace testing;
using namespace std;

#define TEST_BEGIN try {
#define TEST_END } catch(exception &e) { FAIL() << e << endl; }

namespace {
	TEST(UVGetAddrInfo, Create) {
		TEST_BEGIN
			bool success = false;
			GetAddrInfo info("localhost", "http", SOCK_STREAM, [&success](auto &){
				success = true;
			});

			EXPECT_FALSE(success);
		TEST_END
	}

	TEST(UVGetAddrInfo, Run) {
		TEST_BEGIN
			Loop loop;
			bool success = false;
			GetAddrInfo info("localhost", "http", SOCK_STREAM, [&success](auto &){
				success = true;
			});
			loop.Add(info);

			EXPECT_FALSE(success);
			loop();
			EXPECT_TRUE(success);
		TEST_END
	}

	// 2017-10-08 AMR TODO: Causes an Access Violation in Windows
#	ifdef _WIN32
		TEST(UVGetAddrInfo, DISABLED_Cancel) {
#	else
		TEST(UVGetAddrInfo, Cancel) {
#	endif
		TEST_BEGIN
			Loop loop;
			bool success = true;
			auto info = make_shared<GetAddrInfo>("localhost", "http", SOCK_STREAM, [&success](auto &) {
				success = false;
			});
			loop.Add(*info);
			info.reset();

			loop();
			EXPECT_TRUE(success);
		TEST_END
	}
}
