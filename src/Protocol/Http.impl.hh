/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_HTTP_IMPL
#	define ENTROPY_TETHYS_PROTOCOL_HTTP_IMPL

#	include "Http.hh"

#	include <cctype>

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<typename App, typename charT, typename traits>
				Http<App, charT, traits>::Http(App &a) :
					_line(*this),
					_application(a),
					_message(),
					_is_body(false)
				{}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Http<App, charT, traits>::onConnect(Sock &s)
				{
					_application.onConnect(s);
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Http<App, charT, traits>::onDisconnect(Sock &s)
				{
					_application.onDisconnect(s);
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Http<App, charT, traits>::onEof(Sock &s)
				{
					if(_message) {
						_application.getApplication().onMessage(s, std::move(*_message));
						_message.reset();
						_is_body = false;
					}

					_application.onEof(s);
				}

				template<typename App, typename charT, typename traits>
				void Http<App, charT, traits>::onError(const Entropy::Exception &e)
				{
					_application.onError(e);
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Http<App, charT, traits>::onData(Sock &s)
				{
					if(!_message || !_is_body) {
						_line.onData(s);
					}

					if(_message && _is_body) {
						_message->addBody(s);

						if(s.peek() == '\r') {
							s.seekg(1, std::ios_base::cur);
							if(s.peek() == '\n') {
								s.seekg(1, std::ios_base::cur);
								_application.getApplication().onMessage(s, std::move(*_message));
								_message.reset();
								_is_body = false;
							} else {
								s.seekg(-1, std::ios_base::cur);
							}
						}
					}
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Http<App, charT, traits>::onLine(Sock &s, string_type &&line)
				{
					// 2018-02-15 AMR NOTE: onLine will not get called if _is_body
					if(!_message) {
						_message = std::make_shared<HttpMessage<char_type, traits_type>>(std::move(line), ParserLine);
					} else if(line != "") {
						_message->addHeader(std::move(line));
					} else if(_message->expectsBody()) {
						_is_body = true;
					} else {
						_application.getApplication().onMessage(s, std::move(*_message));
						_message.reset();
					}
				}

				template<typename App, typename charT, typename traits>
				bool Http<App, charT, traits>::expectsLine()
				{
					return !_is_body;
				}
			}
		}
	}

#endif
