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
				template<typename Application, typename Socket, typename charT>
				Line<Application, Socket, charT>::Line(Application &app, const std::basic_string<charT> &delim)
					: _app(app), _delim(delim)
				{}

				template<typename Application, typename Socket, typename charT> Line<Application, Socket, charT>::Line(const Line<Application, Socket, charT> &) = default;
				template<typename Application, typename Socket, typename charT> Line<Application, Socket, charT>::Line(Line<Application, Socket, charT> &&) = default;
				template<typename Application, typename Socket, typename charT> Line<Application, Socket, charT>::~Line() = default;

				template<typename Application, typename Socket, typename charT>
				void Line<Application, Socket, charT>::onConnect(Socket &s)
				{
					detail::call_connect<Application, detail::has_connect<Application, void(Socket &s)>::value>::apply(App(), s);
				}

				template<typename Application, typename Socket, typename charT>
				void Line<Application, Socket, charT>::onDisconnect(Socket &s)
				{
					detail::call_disconnect<Application, detail::has_disconnect<Application, void(Socket &s)>::value>::apply(App(), s);
				}

				template<typename Application, typename Socket, typename charT>
				void Line<Application, Socket, charT>::onEof(Socket &s)
				{
					detail::call_eof<Application, detail::has_eof<Application, void(Socket &s)>::value>::apply(App(), s);
				}

				template<typename Application, typename Socket, typename charT>
				void Line<Application, Socket, charT>::onError(const Entropy::Exception &e)
				{
					detail::call_error<Application, detail::has_error<Application, void(const Entropy::Exception &)>::value>::apply(App(), e);

					if(!detail::has_error<Application, void(const Entropy::Exception &)>::value)
						throw e;
				}

				template<typename Application, typename Socket, typename charT>
				void Line<Application, Socket, charT>::onData(Socket &s)
				{
					using std::basic_string;

					std::streamsize pos = findLine(s);

					while(pos) {
						basic_string<charT> line(pos, 0);
						s.read(line.data(), pos);

						line = line.substr(0, line.size() - _delim.size());
						App().onLine(s, line);

						pos = findLine(s);
					}
				}

				template<typename Application, typename Socket, typename charT>
				std::streamsize Line<Application, Socket, charT>::findLine(Socket &s)
				{
					auto x = 0u;
					auto y = 0u;

					auto start = s.tellg();

					while(s.peek() != Socket::traits_type::eof()) {
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

				template<typename Application, typename Socket, typename charT>
				Application &Line<Application, Socket, charT>::App()
				{
					return _app;
				}

				template<typename Application, typename Socket, typename charT>
				const Application &Line<Application, Socket, charT>::App() const
				{
					return _app;
				}
			}
		}
	}


#endif
