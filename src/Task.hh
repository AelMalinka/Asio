/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_TASK_INC
#	define ENTROPY_TETHYS_TASK_INC

#	include <functional>
#	include "Exception.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			class Task
			{
				public:
					Task();
					Task(const std::function<void()> &);
					Task(const std::function<void()> &, const std::function<void()> &);
					Task(const Task &);
					Task(Task &&);
					virtual ~Task();
					virtual void operator () ();
					virtual bool hasAfter() const;
					virtual void After();
				private:
					std::function<void()> _task;
					std::function<void()> _after;
			};
		}
	}

#endif
