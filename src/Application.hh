/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_APPLICATION_INC
#	define ENTROPY_TETHYS_APPLICATION_INC

#	include <memory>
#	include <Entropy/Application.hh>
#	include "Loop.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			class Application
				: public Entropy::Application
			{
				public:
					Application();
					Application(const int, char *[]);
					Application(const int, char *[], const std::shared_ptr<Loop> &);
					Application(const int, char *[], std::shared_ptr<Loop> &&);
					virtual ~Application();
					virtual void operator () ();
					virtual void Stop();
					virtual void Add(Task &);
					virtual void setSignal(const int, const std::function<void()> &);
				private:
					std::shared_ptr<Loop> _loop;
			};
		}
	}

#endif
