/*	Copyright 2015 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include PROTOCOL_INCLUDE
#include "../Mock.hh"
#include <gtest/gtest.h>

using namespace std;
using namespace Entropy::Tethys::Protocol;
using namespace testing;

#define TEST_BEGIN try {
#define TEST_END } catch(exception &e) { FAIL() << e; }

namespace {
	TEST(PROTOCOL, Instantiation) {
		TEST_BEGIN
			MOCK_FULL app;
			PROTOCOL<MOCK_FULL> l(app);
		TEST_END
	}

	TEST(PROTOCOL, onConnect) {
		TEST_BEGIN
			MOCK_FULL app;
			PROTOCOL<MOCK_FULL> l(app);
			stringstream sock;

			EXPECT_CALL(app, onConnect(_))
				.Times(1);

			l.onConnect(sock);
		TEST_END
	}

	TEST(PROTOCOL, onConnectNoConnect) {
		TEST_BEGIN
			StrictMock<MOCK_BASE> app;
			PROTOCOL<MOCK_BASE> l(app);
			stringstream sock;

			l.onConnect(sock);
		TEST_END
	}

	TEST(PROTOCOL, onDisconnect) {
		TEST_BEGIN
			MOCK_FULL app;
			PROTOCOL<MOCK_FULL> l(app);
			stringstream sock;

			EXPECT_CALL(app, onDisconnect(_))
				.Times(1);

			l.onDisconnect(sock);
		TEST_END
	}

	TEST(PROTOCOL, onDisconnectNoDisconnect) {
		TEST_BEGIN
			StrictMock<MOCK_BASE> app;
			PROTOCOL<MOCK_BASE> l(app);
			stringstream sock;

			l.onDisconnect(sock);
		TEST_END
	}

	TEST(PROTOCOL, onEof) {
		TEST_BEGIN
			MOCK_FULL app;
			PROTOCOL<MOCK_FULL> l(app);
			stringstream sock;

			EXPECT_CALL(app, onEof(_))
				.Times(1);

			l.onEof(sock);
		TEST_END
	}

	TEST(PROTOCOL, onEofNoEof) {
		TEST_BEGIN
			StrictMock<MOCK_BASE> app;
			PROTOCOL<MOCK_BASE> l(app);
			stringstream sock;

			l.onDisconnect(sock);
		TEST_END
	}

	TEST(PROTOCOL, onError) {
		TEST_BEGIN
			MOCK_FULL app;
			PROTOCOL<MOCK_FULL> l(app);
			Exception err("This is an error");

			EXPECT_CALL(app, onError(_))
				.Times(1);

			l.onError(err);
		TEST_END
	}

	TEST(PROTOCOL, onErrorNoError) {
		StrictMock<MOCK_BASE> app;
		PROTOCOL<MOCK_BASE> l(app);
		Exception err("This is an error");

		// 2018-02-10 AMR NOTE: possibly loses type
		EXPECT_THROW(l.onError(err), Entropy::Exception);
	}
}
