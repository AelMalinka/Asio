/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_SIGNAL_INC
#	define ENTROPY_TETHYS_UV_SIGNAL_INC

#	include <functional>
#	include <uv.h>
#	include "Task.hh"

	extern "C" {
		void _entropy_tethys_uv_signal_signal_cb(uv_signal_t *, int);
	}

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class Loop;

				class Signal :
					public Task
				{
					public:
						Signal(const int, const std::function<void()> &);
						virtual ~Signal();
						virtual void Added(Loop &);
						virtual void setCallback(const std::function<void()> &);
					protected:
						virtual void SignalCb(const int);
					private:
						uv_signal_t _handle;
						int _signum;
						std::function<void()> _cb;
					friend void ::_entropy_tethys_uv_signal_signal_cb(uv_signal_t *, int);
				};
			}
		}
	}

#	include "Loop.hh"

#endif
