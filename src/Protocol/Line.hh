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
					typename charT = char,
					typename traits = std::char_traits<charT>
				>
				class Line
				{
					public:
						typedef charT char_type;
						typedef traits traits_type;
						typedef std::basic_string<char_type, traits_type> string_type;
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
					private:
						Application<App> _application;
						string_type _delim;
				};
			}
		}
	}

#	include "Line.impl.hh"

#endif
