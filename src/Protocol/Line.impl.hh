/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_LINC_IMPL
#	define ENTROPY_TETHYS_PROTOCOL_LINC_IMPL

#	include "Line.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<typename App, typename charT, typename traits>
				Line<App, charT, traits>::Line(App &app, const string_type &delim)
					: _application(app), _delim(delim)
				{}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Line<App, charT, traits>::onConnect(Sock &s)
				{
					_application.onConnect(s);
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Line<App, charT, traits>::onDisconnect(Sock &s)
				{
					_application.onDisconnect(s);
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Line<App, charT, traits>::onEof(Sock &s)
				{
					_application.onEof(s);
				}

				template<typename App, typename charT, typename traits>
				void Line<App, charT, traits>::onError(const Entropy::Exception &e)
				{
					_application.onError(e);
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				void Line<App, charT, traits>::onData(Sock &s)
				{
					using std::basic_string;

					std::streamsize pos = findLine(s);

					while(pos) {
						string_type line(pos, 0);
						auto at = s.tellg();
						s.read(line.data(), pos);

						line = line.substr(0, line.size() - _delim.size());
						ENTROPY_LOG(Log, Severity::Debug) << " start: " << at << " at: " << s.tellg() << " next: " << static_cast<char>(s.peek()) << " left: " << s.rdbuf()->in_avail() << " got: '" << line << "'";
						_application.getApplication().onLine(s, std::move(line));

						pos = findLine(s);
					}
				}

				template<typename App, typename charT, typename traits>
				template<typename Sock>
				std::streamsize Line<App, charT, traits>::findLine(Sock &s)
				{
					auto x = 0u;
					auto y = 0u;

					auto start = s.tellg();

					while(s.peek() != Sock::traits_type::eof()) {
						if(s.peek() == _delim[y]) {
							if(y++ == _delim.size() - 1) {
								s.seekg(start);
								return ++x;
							}
						} else {
							y = 0;
						}

						s.seekg(1, std::ios_base::cur);
						x++;
					}

					s.clear();
					s.seekg(start);
					return 0;
				}
			}
		}
	}

#endif
