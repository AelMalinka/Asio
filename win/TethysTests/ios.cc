/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{
	namespace Tethys = Entropy::Tethys;

	template<typename charT = char>
	class MockStream :
		public basic_iostream<charT>
	{
		public:
			MockStream();
			void Read(Tethys::Buffer<charT> &&);
			void Write(Tethys::Buffer<charT> &&);
			const basic_string<charT> &Data() const;
		private:
			Tethys::StreamBuffer<MockStream<charT>, charT> _buffer;
			basic_string<charT> _data;
	};

	TEST_CLASS(Buffer) {
		TEST_METHOD(Create) {
			const char *data = "Hello World!";
			size_t len = 12;

			char *temp = new char[1024];
			memcpy(temp, data, len);

			Tethys::Buffer<> buffer(len, 1024, temp);

			Assert::AreEqual(buffer.size(), len);
			Assert::AreEqual(buffer.capacity(), 1024u);
			Assert::AreEqual(buffer.data(), temp);
		}

		TEST_METHOD(Iterate) {
			const char *data = "Hello World!";
			size_t len = strlen(data);

			char *temp = new char[1024];
			strcpy(temp, data);

			Tethys::Buffer<> buffer(len, 1024, temp);

			size_t x = 0;
			for (auto &&c : buffer) {
				Assert::AreEqual(c, temp[x]);
				Assert::AreEqual(c, data[x]);
				++x;

				if (c == '!')
					c = '?';
			}

			Assert::AreEqual(strcmp(buffer.data(), temp), 0);
			Assert::AreNotEqual(strcmp(buffer.data(), data), 0);
		}

		TEST_METHOD(Construct) {
			Tethys::Buffer<> buff(1024);

			Assert::AreEqual(buff.size(), 0u);
			Assert::AreEqual(buff.capacity(), 1024u);
			Assert::AreNotEqual(buff.data(), nullptr);

			Tethys::Buffer<> nbuff(move(buff));

			Assert::AreEqual(buff.data(), nullptr);
			Assert::AreNotEqual(nbuff.data(), nullptr);
		}

		TEST_METHOD(Write) {
			Tethys::Buffer<> buff(1024);

			strcpy(buff.data(), "Hello World!");
			buff.size() = 12;

			Assert::AreEqual(buff.size(), 12u);
			Assert::AreEqual(memcmp(buff.data(), "Hello World!", buff.size()), 0);
			Assert::AreEqual(buff.capacity(), 1024u);
		}
	};

	TEST_CLASS(StreamBuffer) {
		TEST_METHOD(Create) {
			MockStream<> mock;
		}

		TEST_METHOD(Read) {
			MockStream<> mock;
			const char *data[] = {
				"Hello World!\n",
				"Goodbye World!\n",
				"ThisIsALongString",
				"ThatWillDefinitely",
				"BeOnMultipleBuffers"
			};

			for (auto x = 0; x < 5; x++) {
				Tethys::Buffer<> buff(256);
				strcpy(buff.data(), data[x]);
				buff.size() = strlen(data[x]);
				mock.Read(move(buff));
			}

			string hello, goodbye, rest;
			getline(mock, hello);
			getline(mock, goodbye);
			mock >> rest;

			Assert::AreEqual(hello, "Hello World!"s);
			Assert::AreEqual(goodbye, "Goodbye World!"s);
			Assert::AreEqual(rest, "ThisIsALongStringThatWillDefinitelyBeOnMultipleBuffers"s);
		}

		TEST_METHOD(Write) {
			MockStream<> mock;

			mock << "Hello Stream!" << endl;
			Assert::AreEqual(mock.Data(), "Hello Stream!\n"s);

			mock << "This is also some data, hopefully will be buffered";
			Assert::AreEqual(mock.Data(), "Hello Stream!\n"s);
			mock << flush;
			Assert::AreEqual(mock.Data(), "Hello Stream!\nThis is also some data, hopefully will be buffered"s);
		}

		TEST_METHOD(Seek) {
			MockStream<> mock;
			const char *data[] = {
				"Hello World!\n",
				"Goodbye World!\n",		// 13
				"ThisIsALongString",	// 28
				"ThatWillDefinitely",	// 45
				"BeOnMultipleBuffers"	// 63
			};

			for (auto x = 0; x < 5; x++) {
				Tethys::Buffer<> buff(256);
				strcpy(buff.data(), data[x]);
				buff.size() = strlen(data[x]);
				mock.Read(move(buff));
			}

			mock.seekg(0);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'H');
			mock.seekg(6);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'W');
			mock.seekg(0);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'H');
			mock.seekg(6, ios_base::cur);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'W');
			mock.seekg(-6, ios_base::cur);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'H');
			mock.seekg(-1, ios_base::end);
			Assert::AreEqual(static_cast<char>(mock.peek()), 's');
			mock.seekg(0);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'H');
			mock.seekg(13);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'G');
			mock.seekg(45);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'T');
			mock.seekg(0);
			Assert::AreEqual(static_cast<char>(mock.peek()), 'H');

			string hello, goodbye, rest;
			getline(mock, hello);
			getline(mock, goodbye);
			mock >> rest;

			Assert::AreEqual(hello, "Hello World!"s);
			Assert::AreEqual(goodbye, "Goodbye World!"s);
			Assert::AreEqual(rest, "ThisIsALongStringThatWillDefinitelyBeOnMultipleBuffers"s);
		}
	};

	template<typename charT>
	void MockStream<charT>::Read(Tethys::Buffer<charT> &&buff)
	{
		_buffer.AddData(std::move(buff));
	}

	template<typename charT>
	void MockStream<charT>::Write(Tethys::Buffer<charT> &&buff)
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
