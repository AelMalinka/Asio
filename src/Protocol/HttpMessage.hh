/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_INC
#	define ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_INC

#	include "Message.hh"
//	2018-02-21 AMR TODO: cleanup
#	include <experimental/array>

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				namespace detail
				{
					using std::experimental::make_array;

					struct line_t {};
				}

				// 2018-02-22 AMR TODO: default user-agent
				// 2018-02-21 AMR TODO: HTTP/2
				// 2018-02-21 AMR TODO: Header types
				// 2018-02-21 AMR TODO: cacheing
				// 2018-02-22 AMR TODO: cookies?
				// 2018-02-22 AMR TODO: support §3.2.4 rejection of folding (optionally?)
				template<
					typename stringT = std::string
				>
				class HttpMessage :
					public Message<stringT>
				{
					public:
						typedef Message<stringT> base;
						typedef typename base::string_type string_type;
					private:
						typedef std::vector<string_type> container_type;
					public:
						// 2018-02-21 AMR TODO: this probably doesn't work when char_type != char
						explicit HttpMessage(const string_type &, const string_type & = "GET", const string_type & = "HTTP/1.1");
						explicit HttpMessage(unsigned, const string_type & = "", const string_type & = "HTTP/1.1");
						explicit HttpMessage(string_type &&, const detail::line_t &);
						HttpMessage(const HttpMessage<stringT> &);
						HttpMessage(HttpMessage<stringT> &&);
						string_type Start() const;
						bool isRequest() const;
						bool isResponse() const;
						bool expectsBody() const;
						unsigned Status() const;
						float Version() const;
						string_type &Method();
						string_type &Url();
						string_type &VersionCode();
						const string_type &Method() const;
						const string_type &Url() const;
						const string_type &VersionCode() const;
						string_type &StatusCode();
						string_type &StatusMessage();
						const string_type &StatusCode() const;
						const string_type &StatusMessage() const;
					protected:
						container_type &Pieces();
						const container_type &Pieces() const;
					private:
						container_type _start_pieces;
						// 2018-02-21 AMR TODO: this probably doesn't work when charT != char
						static constexpr decltype(auto) _methods = detail::make_array(
							"GET",
							"HEAD",
							"POST",
							"PUT",
							"DELETE",
							"CONNECT",
							"OPTIONS",
							"TRACE"
						);
						typedef ::boost::error_info<struct tag_CodeInfo, unsigned> CodeInfo;
						typedef typename ::boost::error_info<struct tag_VersionCodeInfo, string_type> VersionCodeInfo;
						typedef typename ::boost::error_info<struct tag_StatusCodeInfo, string_type> StatusCodeInfo;
						typedef typename ::boost::error_info<struct tag_StartLineInfo, string_type> StartLineInfo;
				};

				extern detail::line_t ParserLine;
			}
		}
	}

	template<typename charT = char, typename traits = std::char_traits<charT>, typename stringT = std::basic_string<charT, traits>>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &, const Entropy::Tethys::Protocol::HttpMessage<stringT> &);

#	include "HttpMessage.impl.hh"

#endif
