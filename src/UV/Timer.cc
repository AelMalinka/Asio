/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Timer.hh"

using namespace Entropy::Tethys::UV;
using namespace std;
using namespace std::chrono;

Timer::~Timer()
{
	try
	{
		Stop();
	}
	catch(exception &e)
	{}
}

void Timer::Added(Loop &loop)
{
	ThrowIfError("Failed to initialize timer", uv_timer_init(loop.Handle(), &_handle));
	ThrowIfError("Failed to start timer", uv_timer_start(&_handle, _entropy_tethys_uv_timer_cb, _delay.count(), _repeat.count()));
}

void Timer::Stop()
{
	ThrowIfError("Failed to stop timer", uv_timer_stop(&_handle));
}

void _entropy_tethys_uv_timer_cb(uv_timer_t *handle)
{
	Timer *timer = static_cast<Timer *>(handle->data);
	(*timer)();
}
