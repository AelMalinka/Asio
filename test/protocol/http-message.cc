/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Protocol/HttpMessage.hh"
#include <gtest/gtest.h>
#include <sstream>

using namespace std;
using namespace Entropy::Tethys::Protocol;
using namespace testing;

#define TEST_BEGIN try {
#define TEST_END } catch(exception &e) { FAIL() << e; }

namespace {
	TEST(HttpMessage, InstantiateRequest) {
		TEST_BEGIN
			HttpMessage<> m1("/");
			HttpMessage<> m2("/"s);
			HttpMessage<> m3("/"s, "GET"s, "HTTP/1.1"s);
			HttpMessage<> m4("GET / HTTP/1.1"s, ParserLine);

			EXPECT_FALSE(m1.hasBody());
			EXPECT_FALSE(m2.hasBody());
			EXPECT_FALSE(m3.hasBody());
			EXPECT_FALSE(m4.hasBody());

			EXPECT_TRUE(m1.isRequest());
			EXPECT_TRUE(m2.isRequest());
			EXPECT_TRUE(m3.isRequest());
			EXPECT_TRUE(m4.isRequest());

			EXPECT_FALSE(m1.isResponse());
			EXPECT_FALSE(m2.isResponse());
			EXPECT_FALSE(m3.isResponse());
			EXPECT_FALSE(m4.isResponse());

			EXPECT_FLOAT_EQ(m1.Version(), 1.1);
			EXPECT_FLOAT_EQ(m2.Version(), 1.1);
			EXPECT_FLOAT_EQ(m3.Version(), 1.1);
			EXPECT_FLOAT_EQ(m4.Version(), 1.1);

			EXPECT_EQ(m1.Url(), "/"s);
			EXPECT_EQ(m2.Url(), "/"s);
			EXPECT_EQ(m3.Url(), "/"s);
			EXPECT_EQ(m4.Url(), "/"s);

			EXPECT_EQ(m1.Method(), "GET"s);
			EXPECT_EQ(m2.Method(), "GET"s);
			EXPECT_EQ(m3.Method(), "GET"s);
			EXPECT_EQ(m4.Method(), "GET"s);

			EXPECT_EQ(m1.Start(), "GET / HTTP/1.1"s);
			EXPECT_EQ(m2.Start(), "GET / HTTP/1.1"s);
			EXPECT_EQ(m3.Start(), "GET / HTTP/1.1"s);
			EXPECT_EQ(m4.Start(), "GET / HTTP/1.1"s);
		TEST_END
	}

	TEST(HttpMessage, InstantiateResponse) {
		TEST_BEGIN
			HttpMessage<> m1(200);
			HttpMessage<> m2(200, "OK");
			HttpMessage<> m3(200, "OK"s, "HTTP/1.1"s);
			HttpMessage<> m4("HTTP/1.1 200 OK"s, ParserLine);

			EXPECT_FALSE(m1.hasBody());
			EXPECT_FALSE(m2.hasBody());
			EXPECT_FALSE(m3.hasBody());
			EXPECT_FALSE(m4.hasBody());

			EXPECT_FALSE(m1.isRequest());
			EXPECT_FALSE(m2.isRequest());
			EXPECT_FALSE(m3.isRequest());
			EXPECT_FALSE(m4.isRequest());

			EXPECT_TRUE(m1.isResponse());
			EXPECT_TRUE(m2.isResponse());
			EXPECT_TRUE(m3.isResponse());
			EXPECT_TRUE(m4.isResponse());

			EXPECT_FLOAT_EQ(m1.Version(), 1.1);
			EXPECT_FLOAT_EQ(m2.Version(), 1.1);
			EXPECT_FLOAT_EQ(m3.Version(), 1.1);
			EXPECT_FLOAT_EQ(m4.Version(), 1.1);

			EXPECT_EQ(m1.Status(), 200u);
			EXPECT_EQ(m2.Status(), 200u);
			EXPECT_EQ(m3.Status(), 200u);
			EXPECT_EQ(m4.Status(), 200u);

			EXPECT_EQ(m1.StatusMessage(), "OK"s);
			EXPECT_EQ(m2.StatusMessage(), "OK"s);
			EXPECT_EQ(m3.StatusMessage(), "OK"s);
			EXPECT_EQ(m4.StatusMessage(), "OK"s);

			EXPECT_EQ(m1.Start(), "HTTP/1.1 200 OK"s);
			EXPECT_EQ(m2.Start(), "HTTP/1.1 200 OK"s);
			EXPECT_EQ(m3.Start(), "HTTP/1.1 200 OK"s);
			EXPECT_EQ(m4.Start(), "HTTP/1.1 200 OK"s);
		TEST_END
	}
}
