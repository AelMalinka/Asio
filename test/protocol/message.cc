/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Protocol/Message.hh"
#include <gtest/gtest.h>
#include <sstream>

using namespace std;
using namespace testing;

template<typename stringT = string>
using M = Entropy::Tethys::Protocol::Message<stringT>;

namespace {
	TEST(Message, Instantiation) {
		M<> m;

		EXPECT_EQ(m.Headers().size(), 0ul);
		EXPECT_FALSE(m.hasBody());
		EXPECT_EQ(m.Body().size(), 0ul);
		EXPECT_FALSE(m.hasBody());
	}

	TEST(Message, BasicHeaders) {
		M<> m;
		stringstream s;

		m.addHeader("Some-Random-Header: This is a Header value"s);
		s << m;

		EXPECT_EQ(s.str(), "Some-Random-Header: This is a Header value\r\n\r\n"s);
		EXPECT_EQ(m.Headers().size(), 1ul);
		EXPECT_EQ(m.Headers()["Some-Random-Header"s], "This is a Header value"s);
		EXPECT_FALSE(m.hasBody());
	}

	TEST(Message, HeaderFolded) {
		M<> m1;
		M<> m2;
		stringstream s;

		m1.addHeader("Header: Value"s);
		m1.addHeader(" on multiple lines"s);
		m2.addHeader("Header: Value\r\n\ton multiple lines");
		s << m1 << m2;

		EXPECT_EQ(s.str(), "Header: Value on multiple lines\r\n\r\nHeader: Value on multiple lines\r\n\r\n"s);
		EXPECT_EQ(m1.Headers().size(), 1ul);
		EXPECT_EQ(m2.Headers().size(), 1ul);
		EXPECT_EQ(m1.Headers()["Header"], "Value on multiple lines"s);
		EXPECT_EQ(m2.Headers()["Header"], "Value on multiple lines"s);
		EXPECT_FALSE(m1.hasBody());
		EXPECT_FALSE(m2.hasBody());
	}

	TEST(Message, BasicBody) {
		M<> m;
		stringstream s;

		m.addBody("This is a sample body\r\nwith multiple lines"s);
		s << m;

		EXPECT_EQ(s.str(), "\r\nThis is a sample body\r\nwith multiple lines\r\n"s);
		EXPECT_EQ(m.Headers().size(), 0ul);
		EXPECT_TRUE(m.hasBody());
		EXPECT_EQ(m.Body(), "This is a sample body\r\nwith multiple lines"s);
	}

	TEST(Message, BodyFromStream) {
		M<> m;
		stringstream i;
		stringstream o;

		m.addHeader("Header: Value"s);
		i << "This is a body"s;
		m.addBody(i);
		o << m;

		EXPECT_EQ(o.str(), "Header: Value\r\n\r\nThis is a body\r\n"s);
		EXPECT_EQ(m.Headers().size(), 1ul);
		EXPECT_EQ(m.Headers()["Header"], "Value"s);
		EXPECT_TRUE(m.hasBody());
		EXPECT_EQ(m.Body(), "This is a body"s);
	}

	TEST(Message, HeadersAndBody) {
		M<> m;
		stringstream s;

		m.addHeader("Header: Value"s);
		m.addBody("This is a body"s);
		s << m;

		EXPECT_EQ(s.str(), "Header: Value\r\n\r\nThis is a body\r\n"s);
		EXPECT_EQ(m.Headers().size(), 1ul);
		EXPECT_EQ(m.Headers()["Header"], "Value"s);
		EXPECT_TRUE(m.hasBody());
		EXPECT_EQ(m.Body(), "This is a body"s);
	}
}
