/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Protocol/Http.hh"
#include "../Mock.hh"

using namespace std;
using namespace Entropy::Tethys;
using namespace Entropy::Tethys::Protocol;
using namespace testing;

namespace {
	struct App {
		void onMessage(stringstream &, HttpMessage<> &&);
		shared_ptr<HttpMessage<>> Message;
	};

	TEST(Http, Instantiation) {
		MockApplication app;
		stringstream sock;
		Http<MockApplication> l(app);
	}

	TEST(Http, BasicCall) {
		MockApplication app;
		stringstream sock;
		Http<MockApplication> l(app);

		sock << "GET / HTTP/1.1\r\n";
		sock << "\r\n";

		EXPECT_CALL(app, onMessage(_, _))
			.Times(1);

		l.onData(sock);
	}

	TEST(Http, BasicData) {
		App app;
		stringstream sock;
		Http<App> l(app);

		sock << "GET / HTTP/1.1\r\n";
		sock << "\r\n";

		l.onData(sock);

		ASSERT_TRUE(app.Message);
		EXPECT_EQ(app.Message->Start(), "GET / HTTP/1.1"s);
	}

	void App::onMessage(stringstream &, HttpMessage<> &&m)
	{
		Message = make_shared<HttpMessage<>>(move(m));
	}
}
