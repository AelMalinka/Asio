/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_IMPL
#	define ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_IMPL

#	include "HttpMessage.hh"
#	include <boost/algorithm/string.hpp>
#	include <boost/lexical_cast.hpp>

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<typename charT, typename traits>
				HttpMessage<charT, traits>::HttpMessage(string_type &&start)
					: _start(std::move(start)), _headers(), _body()
				{
					// 2018-02-16 AMR TODO: optimization
					boost::split(_start_pieces, _start, boost::is_any_of(" "));
					if(
						_start_pieces.size() < 3 ||
						_start_pieces[0] == "" ||
						_start_pieces[1] == "" ||
						_start_pieces[2] == ""
					) {
						ENTROPY_THROW(Exception("Malformed first line in Http::HttpMessage"));
					} else if(_start_pieces.size() > 3) {
						auto i = _start_pieces.begin();
						i += 3;
						while(i != _start_pieces.end()) {
							_start_pieces[2] += " " + std::move(*i++);
						}

						_start_pieces.resize(3);
					}
				}

				template<typename charT, typename traits>
				HttpMessage<charT, traits>::~HttpMessage() = default;

				template<typename charT, typename traits>
				void HttpMessage<charT, traits>::addHeader(string_type &&line)
				{
					// 2018-02-16 AMR TODO: optimization
					std::vector<string_type> pieces;
					boost::split(pieces, line, boost::is_any_of(":"));

					if(
						pieces.size() < 2 ||
						pieces[0] == "" || pieces[1] == "" ||
						std::isspace(pieces[0][0])
					) {
						ENTROPY_THROW(Exception("Malformed Header in Http::HttpMessage"));
					} else if(pieces.size() > 2) {
						auto i = pieces.begin();
						i += 2;
						while(i != pieces.end()) {
							pieces[1] += ":" + std::move(*i++);
						}

						pieces.resize(2);
					}

					boost::trim(pieces[1]);

					try
					{
						if(pieces[0] == "Content-Length") {
							_body.reserve(boost::lexical_cast<std::size_t>(pieces[1]));
							ENTROPY_LOG(Log, Severity::Debug) << "body capacity set to " << _body.capacity();
						}
					}
					catch(boost::bad_lexical_cast &)
					{}

					_headers[std::move(pieces[0])] = std::move(pieces[1]);
				}

				template<typename charT, typename traits>
				void HttpMessage<charT, traits>::addBody(std::basic_istream<charT, traits> &s)
				{
					auto start = s.tellg();
					auto sz = s.rdbuf()->in_avail();

					s.seekg(-2, std::ios_base::end);
					if(s.peek() == '\r') {
						s.seekg(1, std::ios_base::cur);
						if(s.peek() == '\n')
							sz -= 2;
					}

					s.seekg(start);
					auto osz = _body.size();
					_body.resize(sz + osz);
					ENTROPY_LOG(Log, Severity::Debug) << "body size set to " << _body.size();
					s.read(_body.data() + osz, sz);
				}

				template<typename charT, typename traits>
				bool HttpMessage<charT, traits>::expectsBody() const
				{
					// 2018-02-15 AMR TODO: improve unexpected body handling
					return Method() != "GET";
				}

				template<typename charT, typename traits>
				const std::basic_string<charT, traits> &HttpMessage<charT, traits>::Method() const
				{
					return _start_pieces[0];
				}

				template<typename charT, typename traits>
				const std::basic_string<charT, traits> &HttpMessage<charT, traits>::Start() const
				{
					return _start;
				}

				template<typename charT, typename traits>
				const std::basic_string<charT, traits> &HttpMessage<charT, traits>::Body() const
				{
					return _body;
				}

				template<typename charT, typename traits>
				std::unordered_map<std::basic_string<charT, traits>, std::basic_string<charT, traits>> &HttpMessage<charT, traits>::Headers()
				{
					return _headers;
				}

				template<typename charT, typename traits>
				const std::unordered_map<std::basic_string<charT, traits>, std::basic_string<charT, traits>> &HttpMessage<charT, traits>::Headers() const
				{
					return _headers;
				}
			}
		}
	}

	template<typename charT, typename traits>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &o, const Entropy::Tethys::Protocol::HttpMessage<charT, traits> &m)
	{
		o << m.Start() << "\r\n";
		for(auto &i : m.Headers()) {
			o << i.first << ": " << i.second << "\r\n";
		}

		o << "\r\n";
		if(m.expectsBody())
			o << m.Body() << "\r\n";
		
		o << std::flush;

		return o;
	}

#endif
