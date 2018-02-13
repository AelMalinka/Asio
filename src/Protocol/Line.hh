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
				template<typename App, typename Sock, typename charT = char>
				class Line :
					public Application<App, Sock>
				{
					public:
						explicit Line(App &, const std::basic_string<charT> & = "\r\n");
						Line(const Line<App, Sock, charT> &);
						Line(Line<App, Sock, charT> &&);
						virtual ~Line();
						void onData(Sock &);
					protected:
						std::streamsize findLine(Sock &);
					private:
						std::string _delim;
				};
			}
		}
	}

#	include "Line.impl.hh"

#endif
