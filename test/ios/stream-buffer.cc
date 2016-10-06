/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "StreamBuffer.hh"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cwchar>

using namespace Entropy::Asio;
using namespace testing;
using namespace std;

namespace {
	template<typename charT = char>
	class Stream
	{
		public:
			void onRead(const function<void(Buffer<charT> &&)> &);
			void Read(Buffer<charT> &&);
		private:
			function<void(Buffer<charT> &&)> _on_read;
	};

	template<typename charT = char>
	class MockStream :
		public basic_iostream<charT>
	{
		public:
			MockStream();
			void Read(Buffer<charT> &&);
		private:
			Stream<charT> _stream;
			StreamBuffer<Stream<charT>, charT> _buffer;
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

	TEST(WStreamBuffer, Create) {
		MockStream<wchar_t> mock;
	}

	TEST(WStreamBuffer, Read) {
		MockStream<wchar_t> mock;
		const wchar_t *data[] = {
			L"Hello World!\n",
			L"Goodbye World!\n",
			L"ThisIsALongString",
			L"ThatWillDefinitely",
			L"BeOnMultipleBuffers"
		};

		for(auto x = 0; x < 5; x++) {
			Buffer<wchar_t> buff(256);
			wcscpy(buff.data(), data[x]);
			buff.size() = wcslen(data[x]);
			mock.Read(move(buff));
		}

		wstring hello,goodbye,rest;
		getline(mock, hello);
		getline(mock, goodbye);
		mock >> rest;

		EXPECT_EQ(hello, L"Hello World!"s);
		EXPECT_EQ(goodbye, L"Goodbye World!"s);
		EXPECT_EQ(rest, L"ThisIsALongStringThatWillDefinitelyBeOnMultipleBuffers"s);
	}

	TEST(WStreamBuffer, Seek) {
		MockStream<wchar_t> mock;
		const wchar_t *data[] = {
			L"Hello World!\n",
			L"Goodbye World!\n",	// 13
			L"ThisIsALongString",	// 28
			L"ThatWillDefinitely",	// 45
			L"BeOnMultipleBuffers"	// 63
		};

		for(auto x = 0; x < 5; x++) {
			Buffer<wchar_t> buff(256);
			wcscpy(buff.data(), data[x]);
			buff.size() = wcslen(data[x]);
			mock.Read(move(buff));
		}

		mock.seekg(0);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'H');
		mock.seekg(6);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'W');
		mock.seekg(0);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'H');
		mock.seekg(6, ios_base::cur);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'W');
		mock.seekg(-6, ios_base::cur);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'H');
		mock.seekg(-1, ios_base::end);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 's');
		mock.seekg(0);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'H');
		mock.seekg(13);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'G');
		mock.seekg(45);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'T');
		mock.seekg(0);
		EXPECT_EQ(static_cast<wchar_t>(mock.peek()), 'H');

		wstring hello,goodbye,rest;
		getline(mock, hello);
		getline(mock, goodbye);
		mock >> rest;

		EXPECT_EQ(hello, L"Hello World!"s);
		EXPECT_EQ(goodbye, L"Goodbye World!"s);
		EXPECT_EQ(rest, L"ThisIsALongStringThatWillDefinitelyBeOnMultipleBuffers"s);
	}

	template<typename charT>
	void Stream<charT>::onRead(const function<void(Buffer<charT> &&)> &f)
	{
		_on_read = f;
	}

	template<typename charT>
	void Stream<charT>::Read(Buffer<charT> &&buff)
	{
		_on_read(move(buff));
	}

	template<typename charT>
	MockStream<charT>::MockStream()
		: basic_iostream<charT>(&_buffer), _stream(), _buffer(_stream)
	{}

	template<typename charT>
	void MockStream<charT>::Read(Buffer<charT> &&buff)
	{
		_stream.Read(move(buff));
	}
}
