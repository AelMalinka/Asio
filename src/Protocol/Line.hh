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
					typename Sock,
					typename charT = char,
					typename traits = std::char_traits<charT>,
					typename Alloc = std::allocator<charT>
				>
				class Line :
					public Application<App, Sock>
				{
					public:
						explicit Line(App &, const std::basic_string<charT, traits, Alloc> & = "\r\n");
						Line(const Line<App, Sock, charT, traits, Alloc> &);
						Line(Line<App, Sock, charT, traits, Alloc> &&);
						virtual ~Line();
						void onData(Sock &);
					protected:
						std::streamsize findLine(Sock &);
					private:
						std::basic_string<charT, traits, Alloc> _delim;
				};
			}
		}
	}

#	include "Line.impl.hh"

#endif
