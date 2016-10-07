/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_TASK_INC
#	define ENTROPY_ASIO_UV_TASK_INC

#	include "../Task.hh"

	namespace Entropy
	{
		namespace Asio
		{
			namespace UV
			{
				class Loop;

				class Task :
					public Asio::Task
				{
					public:
						virtual void Added(Loop &) = 0;
				};
			}
		}
	}

#	include "Loop.hh"

#endif
