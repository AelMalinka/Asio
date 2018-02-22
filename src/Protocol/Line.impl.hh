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
				namespace detail
				{
					template<typename T, typename F, typename = void>
					struct has_expects_line : std::false_type{};

					template<typename T, typename R, typename ...A>
					struct has_expects_line<T, R(A...),
						typename std::enable_if<
							std::is_same<R, void>::value ||
							std::is_convertible<decltype(
								std::declval<T>().expectsLine(std::declval<A>()...)
							), R>::value
						>::type
					> : std::true_type {};

					template<typename A, bool has>
					struct call_expects_line
					{
						template<typename ...Params>
						static auto apply(A &a, Params && ...params)
						{
							return a.expectsLine(std::forward<Params>(params)...);
						}
					};

					template<typename A>
					struct call_expects_line<A, false>
					{
						template<typename ...Params>
						static auto apply(A &, Params && ...)
						{
							return true;
						}
					};
				}

				template<typename App, typename stringT>
				Line<App, stringT>::Line(App &app, const string_type &delim)
					: _application(app), _delim(delim)
				{}

				template<typename App, typename stringT>
				template<typename Sock>
				void Line<App, stringT>::onConnect(Sock &s)
				{
					_application.onConnect(s);
				}

				template<typename App, typename stringT>
				template<typename Sock>
				void Line<App, stringT>::onDisconnect(Sock &s)
				{
					_application.onDisconnect(s);
				}

				template<typename App, typename stringT>
				template<typename Sock>
				void Line<App, stringT>::onEof(Sock &s)
				{
					_application.onEof(s);
				}

				template<typename App, typename stringT>
				void Line<App, stringT>::onError(const Entropy::Exception &e)
				{
					_application.onError(e);
				}

				template<typename App, typename stringT>
				template<typename Sock>
				void Line<App, stringT>::onData(Sock &s)
				{
					std::streamsize pos = findLine(s);

					while(expectsLine() && pos) {
						string_type line(pos, 0);
						s.read(line.data(), pos);

						line = line.substr(0, line.size() - _delim.size());
						_application.getApplication().onLine(s, std::move(line));

						pos = findLine(s);
					}
				}

				template<typename App, typename stringT>
				template<typename Sock>
				std::streamsize Line<App, stringT>::findLine(Sock &s)
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

				template<typename App, typename stringT>
				bool Line<App, stringT>::expectsLine()
				{
					return detail::call_expects_line<App, detail::has_expects_line<App, bool()>::value>::apply(_application.getApplication());
				}
			}
		}
	}

#endif
