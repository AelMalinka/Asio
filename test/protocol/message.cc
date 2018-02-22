/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Protocol/Message.hh"
#include <gtest/gtest.h>
#include <sstream>

using namespace std;
using namespace testing;

template<typename charT = char, typename traits = char_traits<charT>>
using M = Entropy::Tethys::Protocol::Message<charT, traits>;

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
