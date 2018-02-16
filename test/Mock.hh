/*	Copyright 2015 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_TEST_PROTOCOL_MOCK_INC
#	define ENTROPY_TETHYS_TEST_PROTOCOL_MOCK_INC

#	include <gmock/gmock.h>
#	include "Stream.hh"

//	2018-02-14 AMR TODO: clean this up
#	include "Protocol/HttpMessage.hh"

	namespace {
		template<typename charT = char>
		class MockStream :
			public Entropy::Tethys::Stream<charT>
		{
			public:
				MockStream();
				void Write(Entropy::Tethys::Buffer<charT> &&);
				void onData(Entropy::Tethys::Stream<charT> &) {}
				const std::basic_string<charT> &Data() const;
			private:
				std::basic_string<charT> _data;
		};

		class MockApplication {
			public:
				MOCK_METHOD1(onConnect, void(std::stringstream &));
				MOCK_METHOD1(onDisconnect, void(std::stringstream &));
				MOCK_METHOD1(onEof, void(std::stringstream &));
				MOCK_METHOD2(onLine, void(std::stringstream &, const std::string &));
				MOCK_METHOD1(onError, void(const Entropy::Exception &));
				MOCK_METHOD2(onMessage, void(std::stringstream &, const Entropy::Tethys::Protocol::HttpMessage<> &));
		};

		class MockApplicationEmpty {
			public:
				MOCK_METHOD2(onLine, void(std::stringstream &, const std::string &));
				MOCK_METHOD2(onMessage, void(std::stringstream &, const Entropy::Tethys::Protocol::HttpMessage<> &));
		};

		template<typename charT>
		void MockStream<charT>::Write(Entropy::Tethys::Buffer<charT> &&buff)
		{
			_data += std::string(buff.data(), buff.size());
		}

		template<typename charT>
		const std::basic_string<charT> &MockStream<charT>::Data() const
		{
			return _data;
		}

		template<typename charT>
		MockStream<charT>::MockStream() : 
			Entropy::Tethys::Stream<charT>(*this)
		{}
	}

#endif
