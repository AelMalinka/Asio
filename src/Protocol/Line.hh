/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_LINE_INC
#	define ENTROPY_TETHYS_PROTOCOL_LINE_INC

#	include "Exception.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<typename Application, typename Socket, typename charT = char>
				class Line
				{
					public:
						explicit Line(Application &, const std::basic_string<charT> & = "\r\n");
						Line(const Line<Application, Socket, charT> &);
						Line(Line<Application, Socket, charT> &&);
						virtual ~Line();
						void onConnect(Socket &);
						void onDisconnect(Socket &);
						void onEof(Socket &);
						void onError(const Entropy::Exception &);
						void onData(Socket &);
					protected:
						std::streamsize findLine(Socket &);
						Application &App();
						const Application &App() const;
					private:
						Application &_app;
						std::string _delim;
				};
			}
		}
	}

#	include "Line.impl.hh"

#endif
