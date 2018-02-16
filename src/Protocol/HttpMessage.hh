/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_INC
#	define ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_INC


	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<
					typename charT = char,
					typename traits = std::char_traits<charT>
				>
				class HttpMessage
				{
					public:
						typedef charT char_type;
						typedef traits traits_type;
						typedef std::basic_string<char_type, traits_type> string_type;
					public:
						explicit HttpMessage(string_type &&);
						virtual ~HttpMessage();
						operator string_type() const;
						void addHeader(string_type &&);
						void addBody(std::basic_istream<charT, traits> &);
						std::unordered_map<string_type, string_type> &Headers();
						const std::unordered_map<string_type, string_type> &Headers() const;
						const string_type &Start() const;
						const string_type &Body() const;
						bool expectsBody() const;
						const string_type &Method() const;
					private:
						string_type _start;
						std::unordered_map<string_type, string_type> _headers;
						string_type _body;
						std::vector<string_type> _start_pieces;
				};
			}
		}
	}

	template<typename charT = char, typename traits = std::char_traits<charT>>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &, const Entropy::Tethys::Protocol::HttpMessage<charT, traits> &);

#	include "HttpMessage.impl.hh"

#endif
