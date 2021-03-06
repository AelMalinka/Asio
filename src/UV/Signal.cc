/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Signal.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Log;
using Entropy::Severity;

Signal::Signal(const int s, const function<void()> &c)
	: _handle(), _signum(s), _cb(c)
{
	_handle.data = this;
}

Signal::~Signal()
{
	try
	{
		Stop();
	}
	catch(exception &)
	{}
}

void Signal::Stop()
{
	ThrowIfError("failed to stop signal", uv_signal_stop(&_handle));
}

void Signal::Added(Loop &loop)
{
	ThrowIfError("failed to initialize signal", uv_signal_init(loop.Handle(), &_handle));
	ThrowIfError("failed to add signal to event loop", uv_signal_start(&_handle, _entropy_tethys_uv_signal_signal_cb, _signum));

	// 2018-02-06 AMR NOTE: don't hold application open for signals
	uv_unref(reinterpret_cast<uv_handle_t *>(&_handle));
}

void Signal::setCallback(const function<void()> &cb)
{
	_cb = cb;
}

void Signal::SignalCb(const int signum)
{
	if(signum == _signum)
		_cb();
}

void _entropy_tethys_uv_signal_signal_cb(uv_signal_t *handle, int signum)
{
	ENTROPY_LOG(Log, Severity::Info) << "Received signal " << signum;

	Signal *signal = static_cast<Signal *>(handle->data);
	signal->SignalCb(signum);
}
