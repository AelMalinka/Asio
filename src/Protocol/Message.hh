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
				// 2018-02-22 AMR TODO: Line Length Limits, §2.1.1 vs 7230 §3.2.5
				template<
					typename stringT = std::string
				>
				class Message
				{
					public:
						typedef stringT string_type;
						typedef typename string_type::value_type char_type;
						typedef typename string_type::traits_type traits_type;
					private:
						typedef std::basic_istream<char_type, traits_type> sock_type;
						typedef std::unordered_map<string_type, string_type> container_type;
					public:
						Message();
						Message(const Message<stringT> &);
						Message(Message<stringT> &&);
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
						typename container_type::iterator _last;
						string_type _body;
						bool _has_body;
						typedef typename ::boost::error_info<struct tag_HeaderLineInfo, string_type> HeaderLineInfo;
				};
			}
		}
	}

	template<typename charT = char, typename traits = std::char_traits<charT>, typename stringT = std::basic_string<charT, traits>>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &, const Entropy::Tethys::Protocol::Message<stringT> &);

#	include "Message.impl.hh"

#endif
