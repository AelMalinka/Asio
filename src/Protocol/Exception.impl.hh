/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_EXCEPTION_IMPL
#	define ENTROPY_TETHYS_PROTOCOL_EXCEPTION_IMPL

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
				}
			}
		}
	}

#endif
