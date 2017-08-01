/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TIMER_IMPL
#	define ENTROPY_TETHYS_UV_TIMER_IMPL

#	include "Timer.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				template<typename DelayType, typename RepeatType>
				Timer::Timer(const DelayType &d, const RepeatType &r, const std::function<void()> &cb) :
					Task(cb),
					_handle(),
					_delay(std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::ratio<1, 1000>>>(d)),
					_repeat(std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::ratio<1, 1000>>>(r))
				{
					_handle.data = this;
				}

				template<typename RepeatType>
				Timer::Timer(const RepeatType &r, const std::function<void()> &cb)
					: Timer(0, r, cb)
				{}

				template<typename DelayType, typename RepeatType>
				Timer::Timer(const DelayType &d, const RepeatType &r) :
					Task(),
					_handle(),
					_delay(std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::ratio<1, 1000>>>(d)),
					_repeat(std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::ratio<1, 1000>>>(r))
				{
					_handle.data = this;
				}

				template<typename RepeatType>
				Timer::Timer(const RepeatType &r) :
					Timer(RepeatType(0), r)
				{}
			}
		}
	}

#endif
