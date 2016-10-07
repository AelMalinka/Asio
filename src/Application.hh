/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_APPLICATION_INC
#	define ENTROPY_ASIO_APPLICATION_INC

#	include <memory>
#	include <Entropy/Application.hh>
#	include "Loop.hh"

	namespace Entropy
	{
		namespace Asio
		{
			class Application
				: public Entropy::Application
			{
				public:
					Application();
					Application(const std::shared_ptr<Loop> &);
					Application(const int, char *[]);
					Application(const int, char *[], const std::shared_ptr<Loop> &);
					virtual ~Application();
					virtual void operator () ();
					virtual void Add(Task &);
				private:
					std::shared_ptr<Loop> _loop;
			};
		}
	}

#endif
