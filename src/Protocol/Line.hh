/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_LINE_INC
#	define ENTROPY_TETHYS_PROTOCOL_LINE_INC

#	include "Application.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<
					typename App,
					typename stringT = std::string
				>
				class Line
				{
					public:
						typedef stringT string_type;
						typedef typename string_type::value_type char_type;
						typedef typename string_type::traits_type traits_type;
					public:
						// 2018-02-15 AMR TODO: does this work on wide and/or utf strings, if not fix
						explicit Line(App &, const string_type & = "\r\n");
						template<typename Sock>
						void onConnect(Sock &);
						template<typename Sock>
						void onDisconnect(Sock &);
						template<typename Sock>
						void onEof(Sock &);
						template<typename Sock>
						void onData(Sock &);
						void onError(const Entropy::Exception &);
					protected:
						template<typename Sock>
						std::streamsize findLine(Sock &);
						bool expectsLine();
					private:
						Application<App> _application;
						string_type _delim;
				};
			}
		}
	}

#	include "Line.impl.hh"

#endif
