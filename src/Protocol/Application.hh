/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_APPLICATION_INC
#	define ENTROPY_TETHYS_PROTOCOL_APPLICATION_INC

#	include "Exception.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<typename App>
				class Application
				{
					public:
						explicit Application(App &);
						template<typename Sock>
						void onConnect(Sock &);
						template<typename Sock>
						void onDisconnect(Sock &);
						template<typename Sock>
						void onEof(Sock &);
						template<typename Sock>
						void onData(Sock &);
						void onError(const Entropy::Exception &);
						App &getApplication();
						const App &getApplication() const;
					private:
						App *_app;
				};
			}
		}
	}

#	include "Application.impl.hh"

#endif
