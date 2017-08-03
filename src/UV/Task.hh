/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TASK_INC
#	define ENTROPY_TETHYS_UV_TASK_INC

#	include "../Task.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class Loop;

				class Task :
					public Tethys::Task
				{
					public:
						Task();
						Task(const std::function<void()> &);
						virtual ~Task();
						virtual void Added(Loop &) = 0;
				};
			}
		}
	}

#endif
