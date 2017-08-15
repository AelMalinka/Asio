/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_LOOP_INC
#	define ENTROPY_TETHYS_UV_LOOP_INC

#	include "../Loop.hh"
#	include <map>
#	include <memory>
#	include "Signal.hh"
#	include "Timer.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				void ThrowIfError(const std::string &, const int);

				template<typename E>
				typename ::Entropy::detail::WithErrorInfo<E>::type AttachUvInfo(const E &, const int);

				class Loop :
					public Tethys::Loop
				{
					public:
						Loop();
						virtual ~Loop();
						virtual void Add(Tethys::Task &);
						virtual void Stop();
						virtual void operator () ();
						virtual void setSignal(const int, const std::function<void()> &);
						uv_loop_t *Handle();
					private:
						uv_loop_t _loop;
						std::map<int, std::shared_ptr<Signal>> _signals;
						Timer _check_signals;
				};
			}
		}
	}

#	include "Loop.impl.hh"

#endif
