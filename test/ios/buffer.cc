/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Buffer.hh"
#include <gtest/gtest.h>

using namespace Entropy::Asio;
using namespace testing;
using namespace std;

namespace {
	TEST(Buffer, Create) {
		const char *data = "Hello World!";
		size_t len = strlen(data);

		vector<char> temp;
		temp.reserve(1024);
		strcpy(temp.data(), data);

		Buffer<> buffer(1024, len, temp.data());

		EXPECT_EQ(buffer.length(), len);
		EXPECT_EQ(buffer.size(), 1024);
		EXPECT_EQ(buffer.data(), temp.data());
	}

	TEST(Buffer, WCreate) {
		const wchar_t *data = L"Hello World!";
		size_t len = wcslen(data);

		vector<wchar_t> temp;
		temp.reserve(1024);
		memcpy(temp.data(), data, len);

		Buffer<wchar_t> buffer(1024, len, temp.data());

		EXPECT_EQ(buffer.length(), len);
		EXPECT_EQ(buffer.size(), 1024);
		EXPECT_EQ(buffer.data(), temp.data());
	}

	TEST(Buffer, Iterate) {
		const char *data = "Hello World!";
		size_t len = strlen(data);

		vector<char> temp;
		temp.reserve(1024);
		strcpy(temp.data(), data);

		Buffer<> buffer(1024, len, temp.data());

		size_t x = 0;
		for(auto &&c : buffer) {
			EXPECT_EQ(c, temp[x]);
			EXPECT_EQ(c, data[x]);
			++x;

			if(c == '!')
				c = '?';
		}

		EXPECT_EQ(strcmp(buffer.data(), temp.data()), 0);
		EXPECT_NE(strcmp(buffer.data(), data), 0);
	}
}
