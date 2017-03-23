/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "UV/GetAddrInfo.hh"
#include <gtest/gtest.h>

using namespace Entropy::Asio::UV;
using namespace testing;
using namespace std;

namespace {
	TEST(UVGetAddrInfo, Create) {
		bool success = false;
		GetAddrInfo info("localhost", "http", SOCK_STREAM, [&success](auto &){
			success = true;
		});

		EXPECT_FALSE(success);
	}

	TEST(UVGetAddrInfo, Run) {
		Loop loop;
		bool success = false;
		GetAddrInfo info("localhost", "http", SOCK_STREAM, [&success](auto &){
			success = true;
		});
		loop.Add(info);

		EXPECT_FALSE(success);
		loop();
		EXPECT_TRUE(success);
	}
}
