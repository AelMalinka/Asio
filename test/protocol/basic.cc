/*	Copyright 2015 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include PROTOCOL_INCLUDE
#include "../Mock.hh"
#include <gtest/gtest.h>

using namespace std;
using namespace Entropy::Tethys::Protocol;
using namespace testing;

namespace {
	TEST(PROTOCOL, Instantiation) {
		MOCK_FULL app;
		PROTOCOL<MOCK_FULL, stringstream> l(app);
	}

	TEST(PROTOCOL, onConnect) {
		MOCK_FULL app;
		PROTOCOL<MOCK_FULL, stringstream> l(app);
		stringstream sock;

		EXPECT_CALL(app, onConnect(_))
			.Times(1);

		l.onConnect(sock);
	}

	TEST(PROTOCOL, onConnectNoConnect) {
		StrictMock<MOCK_BASE> app;
		PROTOCOL<MOCK_BASE, stringstream> l(app);
		stringstream sock;

		l.onConnect(sock);
	}

	TEST(PROTOCOL, onDisconnect) {
		MOCK_FULL app;
		PROTOCOL<MOCK_FULL, stringstream> l(app);
		stringstream sock;

		EXPECT_CALL(app, onDisconnect(_))
			.Times(1);

		l.onDisconnect(sock);
	}

	TEST(PROTOCOL, onDisconnectNoDisconnect) {
		StrictMock<MOCK_BASE> app;
		PROTOCOL<MOCK_BASE, stringstream> l(app);
		stringstream sock;

		l.onDisconnect(sock);
	}

	TEST(PROTOCOL, onEof) {
		MOCK_FULL app;
		PROTOCOL<MOCK_FULL, stringstream> l(app);
		stringstream sock;

		EXPECT_CALL(app, onEof(_))
			.Times(1);

		l.onEof(sock);
	}

	TEST(PROTOCOL, onEofNoEof) {
		StrictMock<MOCK_BASE> app;
		PROTOCOL<MOCK_BASE, stringstream> l(app);
		stringstream sock;

		l.onDisconnect(sock);
	}

	TEST(PROTOCOL, onError) {
		MOCK_FULL app;
		PROTOCOL<MOCK_FULL, stringstream> l(app);
		Exception err("This is an error");

		EXPECT_CALL(app, onError(_))
			.Times(1);

		l.onError(err);
	}

	TEST(PROTOCOL, onErrorNoError) {
		StrictMock<MOCK_BASE> app;
		PROTOCOL<MOCK_BASE, stringstream> l(app);
		Exception err("This is an error");

		// 2018-02-10 AMR NOTE: possibly loses type
		EXPECT_THROW(l.onError(err), Entropy::Exception);
	}
}
