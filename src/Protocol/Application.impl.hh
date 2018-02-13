/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_APPLICATION_IMPL
#	define ENTROPY_TETHYS_PROTOCOL_APPLICATION_IMPL

#	include <type_traits>

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				namespace detail
				{
					template<typename T, typename F, typename = void>
					struct has_connect : std::false_type {};

					template<typename T, typename F, typename = void>
					struct has_disconnect : std::false_type {};

					template<typename T, typename F, typename = void>
					struct has_eof : std::false_type {};

					template<typename T, typename F, typename = void>
					struct has_error : std::false_type {};

					template<typename T, typename F, typename = void>
					struct has_data : std::false_type {};

					template<typename T, typename R, typename ...A>
					struct has_connect<T, R(A...),
						typename std::enable_if<
							std::is_same<R, void>::value ||
							std::is_convertible<decltype(
								std::declval<T>().onConnect(std::declval<A>()...)
							), R>::value
						>::type
					> : std::true_type {};

					template<typename T, typename R, typename ...A>
					struct has_disconnect<T, R(A...),
						typename std::enable_if<
							std::is_same<R, void>::value ||
							std::is_convertible<decltype(
								std::declval<T>().onDisconnect(std::declval<A>()...)
							), R>::value
						>::type
					> : std::true_type {};

					template<typename T, typename R, typename ...A>
					struct has_eof<T, R(A...),
						typename std::enable_if<
							std::is_same<R, void>::value ||
							std::is_convertible<decltype(
								std::declval<T>().onEof(std::declval<A>()...)
							), R>::value
						>::type
					> : std::true_type {};

					template<typename T, typename R, typename ...A>
					struct has_error<T, R(A...),
						typename std::enable_if<
							std::is_same<R, void>::value ||
							std::is_convertible<decltype(
								std::declval<T>().onError(std::declval<A>()...)
							), R>::value
						>::type
					> : std::true_type {};

					template<typename T, typename R, typename ...A>
					struct has_data<T, R(A...),
						typename std::enable_if<
							std::is_same<R, void>::value ||
							std::is_convertible<decltype(
								std::declval<T>().onData(std::declval<A>()...)
							), R>::value
						>::type
					> : std::true_type {};

					template<typename A, bool Has>
					struct call_connect
					{
						template<typename ...Params>
						static auto apply(A &a, Params && ...params)
						{
							return a.onConnect(std::forward<Params>(params)...);
						}
					};

					template<typename A, bool Has>
					struct call_disconnect
					{
						template<typename ...Params>
						static auto apply(A &a, Params && ...params)
						{
							return a.onDisconnect(std::forward<Params>(params)...);
						}
					};

					template<typename A, bool Has>
					struct call_eof
					{
						template<typename ...Params>
						static auto apply(A &a, Params && ...params)
						{
							return a.onEof(std::forward<Params>(params)...);
						}
					};

					template<typename A, bool Has>
					struct call_error
					{
						template<typename ...Params>
						static auto apply(A &a, Params && ...params)
						{
							return a.onError(std::forward<Params>(params)...);
						}
					};

					template<typename A, bool Has>
					struct call_data
					{
						template<typename ...Params>
						static auto apply(A &a, Params && ...params)
						{
							return a.onData(std::forward<Params>(params)...);
						}
					};

					template<typename A>
					struct call_connect<A, false>
					{
						template<typename ...Params>
						static void apply(A &, Params && ...)
						{}
					};

					template<typename A>
					struct call_disconnect<A, false>
					{
						template<typename ...Params>
						static void apply(A &, Params && ...)
						{}
					};

					template<typename A>
					struct call_eof<A, false>
					{
						template<typename ...Params>
						static void apply(A &, Params && ...)
						{}
					};

					template<typename A>
					struct call_error<A, false>
					{
						template<typename ...Params>
						static void apply(A &, Params && ...)
						{}
					};

					template<typename A>
					struct call_data<A, false>
					{
						template<typename ...Params>
						static void apply(A &, Params && ...)
						{}
					};
				}

				template<typename App, typename Sock>
				Application<App, Sock>::Application(App &app) :
					_app(app)
				{}

				template<typename App, typename Sock> Application<App, Sock>::Application(const Application<App, Sock> &) = default;
				template<typename App, typename Sock> Application<App, Sock>::Application(Application<App, Sock> &&) = default;
				template<typename App, typename Sock> Application<App, Sock>::~Application() = default;

				template<typename App, typename Sock>
				void Application<App, Sock>::onConnect(Sock &s)
				{
					detail::call_connect<App, detail::has_connect<App, void(Sock &s)>::value>::apply(getApplication(), s);
				}

				template<typename App, typename Sock>
				void Application<App, Sock>::onDisconnect(Sock &s)
				{
					detail::call_disconnect<App, detail::has_disconnect<App, void(Sock &s)>::value>::apply(getApplication(), s);
				}

				template<typename App, typename Sock>
				void Application<App, Sock>::onEof(Sock &s)
				{
					detail::call_eof<App, detail::has_eof<App, void(Sock &s)>::value>::apply(getApplication(), s);
				}

				template<typename App, typename Sock>
				void Application<App, Sock>::onError(const Entropy::Exception &e)
				{
					detail::call_error<App, detail::has_error<App, void(const Entropy::Exception &)>::value>::apply(getApplication(), e);

					if(!detail::has_error<App, void(const Entropy::Exception &)>::value)
						throw e;
				}

				template<typename App, typename Sock>
				void Application<App, Sock>::onData(Sock &s)
				{
					detail::call_data<App, detail::has_data<App, void(Sock &s)>::value>::apply(getApplication(), s);
				}

				template<typename App, typename Sock>
				App &Application<App, Sock>::getApplication()
				{
					return _app;
				}

				template<typename App, typename Sock>
				const App &Application<App, Sock>::getApplication() const
				{
					return _app;
				}
			}
		}
	}
#endif
