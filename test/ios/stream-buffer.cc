/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "StreamBuffer.hh"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>

using namespace Entropy::Tethys;
using namespace testing;
using namespace std;

namespace {
	template<typename charT = char>
	class MockStream :
		public basic_iostream<charT>
	{
		public:
			MockStream();
			void Read(Buffer<charT> &&);
			void Write(Buffer<charT> &&);
			const basic_string<charT> &Data() const;
		private:
			StreamBuffer<MockStream<charT>, charT> _buffer;
			basic_string<charT> _data;
	};

	TEST(StreamBuffer, Create) {
		MockStream<> mock;
	}

	TEST(StreamBuffer, Read) {
		MockStream<> mock;
		const char *data[] = {
			"Hello World!\n",
			"Goodbye World!\n",
			"ThisIsALongString",
			"ThatWillDefinitely",
			"BeOnMultipleBuffers"
		};

		for(auto x = 0; x < 5; x++) {
			Buffer<> buff(256);
			strcpy(buff.data(), data[x]);
			buff.size() = strlen(data[x]);
			mock.Read(move(buff));
		}

		string hello,goodbye,rest;
		getline(mock, hello);
		getline(mock, goodbye);
		mock >> rest;

		EXPECT_EQ(hello, "Hello World!"s);
		EXPECT_EQ(goodbye, "Goodbye World!"s);
		EXPECT_EQ(rest, "ThisIsALongStringThatWillDefinitelyBeOnMultipleBuffers"s);
	}

	TEST(StreamBuffer, Write) {
		MockStream<> mock;

		mock << "Hello Stream!" << endl;
		EXPECT_EQ(mock.Data(), "Hello Stream!\n"s);

		mock << "This is also some data, hopefully will be buffered";
		EXPECT_EQ(mock.Data(), "Hello Stream!\n"s);
		mock << flush;
		EXPECT_EQ(mock.Data(), "Hello Stream!\nThis is also some data, hopefully will be buffered"s);
	}

	TEST(StreamBuffer, Seek) {
		MockStream<> mock;
		const char *data[] = {
			"Hello World!\n",
			"Goodbye World!\n",		// 13
			"ThisIsALongString",	// 28
			"ThatWillDefinitely",	// 45
			"BeOnMultipleBuffers"	// 63
		};

		for(auto x = 0; x < 5; x++) {
			Buffer<> buff(256);
			strcpy(buff.data(), data[x]);
			buff.size() = strlen(data[x]);
			mock.Read(move(buff));
		}

		mock.seekg(0);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'H');
		mock.seekg(6);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'W');
		mock.seekg(0);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'H');
		mock.seekg(6, ios_base::cur);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'W');
		mock.seekg(-6, ios_base::cur);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'H');
		mock.seekg(-1, ios_base::end);
		EXPECT_EQ(static_cast<char>(mock.peek()), 's');
		mock.seekg(0);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'H');
		mock.seekg(13);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'G');
		mock.seekg(45);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'T');
		mock.seekg(0);
		EXPECT_EQ(static_cast<char>(mock.peek()), 'H');

		string hello,goodbye,rest;
		getline(mock, hello);
		getline(mock, goodbye);
		mock >> rest;

		EXPECT_EQ(hello, "Hello World!"s);
		EXPECT_EQ(goodbye, "Goodbye World!"s);
		EXPECT_EQ(rest, "ThisIsALongStringThatWillDefinitelyBeOnMultipleBuffers"s);
	}

	// 2017-06-27 AMR TODO: test utf-32, maybe utf-16

	template<typename charT>
	void MockStream<charT>::Read(Buffer<charT> &&buff)
	{
		_buffer.AddData(move(buff));
	}

	template<typename charT>
	void MockStream<charT>::Write(Buffer<charT> &&buff)
	{
		_data += string(buff.data(), buff.size());
	}

	template<typename charT>
	const basic_string<charT> &MockStream<charT>::Data() const
	{
		return _data;
	}

	template<typename charT>
	MockStream<charT>::MockStream()
		: basic_iostream<charT>(&_buffer), _buffer(*this)
	{}
}
