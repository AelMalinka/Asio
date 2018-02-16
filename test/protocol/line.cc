/*	Copyright 2015 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Protocol/Line.hh"
#include "../Mock.hh"

using namespace std;
using namespace Entropy::Tethys;
using namespace Entropy::Tethys::Protocol;
using namespace testing;

namespace {
	TEST(Line, Instantiation) {
		MockApplication app;
		stringstream s;
		Line<MockApplication> l(app);
	}

	TEST(Line, Basic) {
		MockApplication app;
		Line<MockApplication> l(app);

		string s = "Hello World!\r\n"s;
		stringstream sock(s);

		EXPECT_CALL(app, onLine(_, "Hello World!"s))
			.Times(1);

		l.onData(sock);
	}

	TEST(Line, SmallBuffer) {
		MockApplication app;
		stringstream sock;
		Line<MockApplication> l(app);

		string s1 = "Hello"s;
		string s2 = " "s;
		string s3 = "World"s;
		string s4 = "!\r\n"s;

		EXPECT_CALL(app, onLine(_, "Hello World!"s))
			.Times(1);

		sock << s1;
		l.onData(sock);
		sock << s2;
		l.onData(sock);
		sock << s3;
		l.onData(sock);
		sock << s4;
		l.onData(sock);
	}

	TEST(Line, MultipleLines) {
		MockApplication app;
		stringstream sock;
		Line<MockApplication> l(app);

		string s1 = "Hello World!\r\nThis is a message!"s;
		string s2 = "\r\nBecause Mehf\r\n"s;

		EXPECT_CALL(app, onLine(_, "Hello World!"s))
			.Times(1);
		EXPECT_CALL(app, onLine(_, "This is a message!"s))
			.Times(1);
		EXPECT_CALL(app, onLine(_, "Because Mehf"s))
			.Times(1);

		sock << s1;
		l.onData(sock);
		sock << s2;
		l.onData(sock);
	}

	TEST(Line, Delimiter) {
		MockApplication app;
		stringstream sock1;
		stringstream sock2;
		stringstream sock3;
		stringstream sock4;
		stringstream sock5;
		stringstream sock6;
		Line<MockApplication> l1(app, ":");
		Line<MockApplication> l2(app, "\r\n");
		Line<MockApplication> l3(app, "\r");
		Line<MockApplication> l4(app, "\n");
		Line<MockApplication> l5(app, "/");
		Line<MockApplication> l6(app, "/asdfasdf/");

		string s1 = "Hello World!"s;
		string s2 = "This is a Different Delimiter!"s;

		EXPECT_CALL(app, onLine(_, s1))
			.Times(6);
		EXPECT_CALL(app, onLine(_, s2))
			.Times(6);

		string d1 = ":"s;
		string d2 = "\r\n"s;
		string d3 = "\r"s;
		string d4 = "\n"s;
		string d5 = "/"s;
		string d6 = "/asdfasdf/"s;

		string t1 = s1 + d1 + s2 + d1;
		string t2 = s1 + d2 + s2 + d2;
		string t3 = s1 + d3 + s2 + d3;
		string t4 = s1 + d4 + s2 + d4;
		string t5 = s1 + d5 + s2 + d5;
		string t6 = s1 + d6 + s2 + d6;

		sock1 << t1;
		sock2 << t2;
		sock3 << t3;
		sock4 << t4;
		sock5 << t5;
		sock6 << t6;

		l1.onData(sock1);
		l2.onData(sock2);
		l3.onData(sock3);
		l4.onData(sock4);
		l5.onData(sock5);
		l6.onData(sock6);
	}
}
