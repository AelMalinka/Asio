/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_IMPL
#	define ENTROPY_TETHYS_PROTOCOL_HTTPMESSAGE_IMPL

#	include "HttpMessage.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				namespace detail
				{
					template<typename string_type>
					string_type getMessage(unsigned code)
					{
						// 2018-02-21 AMR TODO: this probably fails with charT != char
						switch(code) {
							case 100: return "Continue";
							case 101: return "Switching Protocols";
							case 200: return "OK";
							case 201: return "Created";
							case 202: return "Accepted";
							case 203: return "Non-Authoritative Information";
							case 204: return "No Content";
							case 205: return "Reset Content";
							case 206: return "Partial Content";
							case 300: return "Multiple Choices";
							case 301: return "Moved Permanently";
							case 302: return "Found";
							case 303: return "See Other";
							case 304: return "Not Modified";
							case 305: return "Use Proxy";
							case 307: return "Temporary Redirect";
							case 400: return "Bad Request";
							case 401: return "Unauthorized";
							case 402: return "Payment Required";
							case 403: return "Forbidden";
							case 404: return "Not Found";
							case 405: return "Method Not Allowed";
							case 406: return "Not Acceptable";
							case 407: return "Proxy Authentication Required";
							case 408: return "Request Timeout";
							case 409: return "Conflict";
							case 410: return "Gone";
							case 411: return "Length Required";
							case 412: return "Precondition Failed";
							case 413: return "Payload Too Large";
							case 414: return "URI Too Long";
							case 415: return "Unsupported Media Type";
							case 416: return "Range Not Satisfiable";
							case 417: return "Expectation Failed";
							case 426: return "Upgrade Required";
							case 500: return "Internal Server Error";
							case 501: return "Not Implemented";
							case 502: return "Bad Gateway";
							case 503: return "Service Unavailable";
							case 504: return "Gateway Timeout";
							case 505: return "HTTP Version Not Supported";
							default: return "Unknown";
						}
					}
				}

				template<typename stringT>
				HttpMessage<stringT>::HttpMessage(const string_type &url, const string_type &method, const string_type &version)
					: base(), _start_pieces()
				{
					_start_pieces.resize(3);

					_start_pieces[0] = method;
					_start_pieces[1] = url;
					_start_pieces[2] = version;
				}

				template<typename stringT>
				HttpMessage<stringT>::HttpMessage(unsigned code, const string_type &message, const string_type &version)
					: base(), _start_pieces()
				{
					_start_pieces.resize(3);

					_start_pieces[0] = version;

					try
					{
						_start_pieces[1] = boost::lexical_cast<string_type>(code);
					}
					catch(boost::bad_lexical_cast &)
					{
						ENTROPY_THROW(Exception("Failed to convert Code to string") <<
							CodeInfo(code)
						);
					}

					if(message == "") {
						_start_pieces[2] = detail::getMessage<string_type>(code);
					} else {
						_start_pieces[2] = message;
					}
				}

				template<typename stringT>
				HttpMessage<stringT>::HttpMessage(string_type &&start, const detail::line_t &)
					: base(), _start_pieces()
				{
					// 2018-02-16 AMR TODO: optimization
					boost::split(_start_pieces, start, boost::is_any_of(string_type(1, ' ')));
					if(
						_start_pieces.size() < 3 ||
						_start_pieces[0] == "" ||
						_start_pieces[1] == "" ||
						_start_pieces[2] == ""
					) {
						ENTROPY_THROW(Exception("Malformed first line in Http::HttpMessage") <<
							StartLineInfo(start)
						);
					} else if(_start_pieces.size() > 3) {
						auto i = _start_pieces.begin();
						i += 3;
						while(i != _start_pieces.end()) {
							_start_pieces[2] += string_type(1, ' ') + std::move(*i++);
						}

						_start_pieces.resize(3);
					}
				}

				template<typename stringT> HttpMessage<stringT>::HttpMessage(const HttpMessage<stringT> &) = default;
				template<typename stringT> HttpMessage<stringT>::HttpMessage(HttpMessage<stringT> &&) = default;

				template<typename stringT>
				typename HttpMessage<stringT>::string_type HttpMessage<stringT>::Start() const
				{
					return
						Pieces()[0] + string_type(1, ' ') +
						Pieces()[1] + string_type(1, ' ') +
						Pieces()[2]
					;
				}

				template<typename stringT>
				bool HttpMessage<stringT>::isRequest() const
				{
					for(auto &&m : _methods) {
						if(m == Pieces()[0])
							return true;
					}

					return false;
				}

				template<typename stringT>
				bool HttpMessage<stringT>::isResponse() const
				{
					return !isRequest();
				}

				template<typename stringT>
				bool HttpMessage<stringT>::expectsBody() const
				{
					using namespace std;

					// 2018-02-15 AMR TODO: improve expected body handling
					if(isRequest()) {
						return Method() != "GET"s;
					} else {
						return Status() != 304;
					}
				}

				template<typename stringT>
				unsigned HttpMessage<stringT>::Status() const
				{
					try
					{
						return boost::lexical_cast<unsigned>(StatusCode());
					}
					catch(boost::bad_lexical_cast &)
					{
						ENTROPY_THROW(Exception("Failed to convert StatusCode to unsigned") <<
							StatusCodeInfo(StatusCode())
						);
					}
				}

				template<typename stringT>
				float HttpMessage<stringT>::Version() const
				{
					using namespace std;
					using namespace boost;

					vector<string_type> v;

					split(v, VersionCode(), is_any_of("/"));

					try
					{
						return lexical_cast<float>(v[1]);
					}
					catch(bad_lexical_cast &)
					{
						ENTROPY_THROW(Exception("Failed to convert Version to float") <<
							VersionCodeInfo(VersionCode())
						);
					}
				}

				template<typename stringT>
				typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::Method()
				{
					if(!isRequest())
						ENTROPY_THROW(Exception("HttpMessage is not a request"));

					return Pieces()[0];
				}

				template<typename stringT>
				typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::Url()
				{
					if(!isRequest())
						ENTROPY_THROW(Exception("HttpMessage is not a request"));

					return Pieces()[1];
				}

				template<typename stringT>
				typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::VersionCode()
				{
					if(isRequest())
						return Pieces()[2];
					else
						return Pieces()[0];
				}

				template<typename stringT>
				const typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::Method() const
				{
					if(!isRequest())
						ENTROPY_THROW(Exception("HttpMessage is not a request"));

					return Pieces()[0];
				}

				template<typename stringT>
				const typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::Url() const
				{
					if(!isRequest())
						ENTROPY_THROW(Exception("HttpMessage is not a request"));

					return Pieces()[1];
				}

				template<typename stringT>
				const typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::VersionCode() const
				{
					if(isRequest())
						return Pieces()[2];
					else
						return Pieces()[0];
				}

				template<typename stringT>
				typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::StatusCode()
				{
					if(isRequest())
						ENTROPY_THROW(Exception("HttpMessage is a request"));

					return Pieces()[1];
				}

				template<typename stringT>
				typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::StatusMessage()
				{
					if(isRequest())
						ENTROPY_THROW(Exception("HttpMessage is a request"));

					return Pieces()[2];
				}

				template<typename stringT>
				const typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::StatusCode() const
				{
					if(isRequest())
						ENTROPY_THROW(Exception("HttpMessage is a request"));

					return Pieces()[1];
				}

				template<typename stringT>
				const typename HttpMessage<stringT>::string_type &HttpMessage<stringT>::StatusMessage() const
				{
					if(isRequest())
						ENTROPY_THROW(Exception("HttpMessage is a request"));

					return Pieces()[2];
				}

				template<typename stringT>
				typename HttpMessage<stringT>::container_type &HttpMessage<stringT>::Pieces()
				{
					return _start_pieces;
				}

				template<typename stringT>
				const typename HttpMessage<stringT>::container_type &HttpMessage<stringT>::Pieces() const
				{
					return _start_pieces;
				}
			}
		}
	}

	template<typename charT, typename traits, typename stringT>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &o, const Entropy::Tethys::Protocol::HttpMessage<stringT> &m)
	{
		using Entropy::Tethys::Protocol::Message;

		o << m.Start() << "\r\n";
		return operator << (o, static_cast<const Message<stringT> &>(m));
	}

#endif
