/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Signal.hh"

using namespace Entropy::Asio::UV;
using namespace std;

Signal::Signal(const int s, const function<void()> &c)
	: _handle(), _signum(s), _cb(c)
{
	_handle.data = this;
}

Signal::~Signal()
{
	uv_signal_stop(&_handle);
}

void Signal::Added(Loop &loop)
{
	ThrowIfError("failed to initialize signal", uv_signal_init(loop.Handle(), &_handle));
	ThrowIfError("failed to add signal to event loop", uv_signal_start(&_handle, _entropy_asio_uv_signal_signal_cb, _signum));
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

void _entropy_asio_uv_signal_signal_cb(uv_signal_t *handle, int signum)
{
	Signal *signal = static_cast<Signal *>(handle->data);
	signal->SignalCb(signum);
}
