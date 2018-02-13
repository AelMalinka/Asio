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
				// 2018-02-13 AMR TODO: determine performance cost of virtual here
				template<typename App, typename Sock>
				class Application
				{
					public:
						explicit Application(App &);
						Application(const Application<App, Sock> &);
						Application(Application<App, Sock> &&);
						virtual ~Application();
						virtual void onConnect(Sock &);
						virtual void onDisconnect(Sock &);
						virtual void onEof(Sock &);
						virtual void onError(const Entropy::Exception &);
						virtual void onData(Sock &);
					protected:
						App &getApplication();
						const App &getApplication() const;
					private:
						App &_app;
				};
			}
		}
	}

#	include "Application.impl.hh"

#endif
