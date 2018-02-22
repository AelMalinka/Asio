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
					typename stringT = std::string
				>
				class Http
				{
					public:
						typedef stringT string_type;
						typedef typename string_type::value_type char_type;
						typedef typename string_type::traits_type traits_type;
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
						Line<Http<App, string_type>, string_type> _line;
						Application<App> _application;
						std::shared_ptr<HttpMessage<string_type>> _message;
						bool _is_body;
				};
			}
		}
	}


#	include "Http.impl.hh"

#endif
