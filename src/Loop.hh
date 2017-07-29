/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_LOOP_INC
#	define ENTROPY_ASIO_LOOP_INC

#	include "Task.hh"
#	include <functional>

	namespace Entropy
	{
		namespace Asio
		{
			class Loop
			{
				public:
					virtual void Add(Task &) = 0;
					virtual void Stop() = 0;
					virtual void operator () () = 0;
					virtual void setSignal(const int, const std::function<void()> &) = 0;
			};
		}
	}

#endif
