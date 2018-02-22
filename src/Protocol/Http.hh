/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_HTTP_INC
#	define ENTROPY_TETHYS_PROTOCOL_HTTP_INC

#	include "Line.hh"
#	include "HttpMessage.hh"

#	include <unordered_map>

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				// 2018-02-21 AMR NOTE: RFC 7230-7235
				template<
					typename App,
					typename charT = char,
					typename traits = std::char_traits<charT>
				>
				class Http
				{
					public:
						typedef charT char_type;
						typedef traits traits_type;
						typedef std::basic_string<char_type, traits_type> string_type;
					public:
						explicit Http(App &);
						template<typename Sock>
						void onConnect(Sock &);
						template<typename Sock>
						void onDisconnect(Sock &);
						template<typename Sock>
						void onEof(Sock &);
						template<typename Sock>
						void onData(Sock &);
						template<typename Sock>
						void onLine(Sock &, string_type &&);
						void onError(const Entropy::Exception &);
						bool expectsLine();
					private:
						Line<Http<App, char_type, traits_type>, char_type, traits_type> _line;
						Application<App> _application;
						std::shared_ptr<HttpMessage<char_type, traits_type>> _message;
						bool _is_body;
				};
			}
		}
	}


#	include "Http.impl.hh"

#endif
