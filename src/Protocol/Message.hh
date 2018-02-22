/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_MESSAGE_PROTOCOL_INC
#	define ENTROPY_TETHYS_PROTOCOL_MESSAGE_PROTOCOL_INC

#	include "Exception.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				// 2018-02-21 AMR NOTE: RFC 5322
				// 2018-02-19 AMR TODO: should we have string_type as a parameter
				// 2018-02-21 AMR TODO: support folding per §2.2.3
				// 2018-02-21 AMR TODO: should we use spirit for parseing these?
				template<
					typename charT = char,
					typename traits = std::char_traits<charT>
				>
				class Message
				{
					public:
						typedef charT char_type;
						typedef traits traits_type;
						typedef std::basic_string<char_type, traits_type> string_type;
						typedef std::basic_istream<char_type, traits_type> sock_type;
					private:
						typedef std::unordered_map<string_type, string_type> container_type;
					public:
						Message();
						Message(const Message<charT, traits> &);
						Message(Message<charT, traits> &&);
						virtual ~Message();
						virtual void addHeader(string_type &&);
						virtual void addBody(string_type &&);
						virtual void addBody(sock_type &);
						bool hasBody() const;
						string_type &Body();
						const string_type &Body() const;
						container_type &Headers();
						const container_type &Headers() const;
					private:
						container_type _headers;
						string_type _body;
						bool _has_body;
						typedef typename ::boost::error_info<struct tag_HeaderLineInfo, string_type> HeaderLineInfo;
				};
			}
		}
	}

	template<typename charT = char, typename traits = std::char_traits<charT>>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &, const Entropy::Tethys::Protocol::Message<charT, traits> &);

#	include "Message.impl.hh"

#endif
