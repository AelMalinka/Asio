/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Loop.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Log;
using Entropy::Severity;

void work(uv_work_t *);
void work_after(uv_work_t *, int);
void timer(Loop &, map<int, shared_ptr<Signal>> &, Timer &);
void walk(uv_handle_t *, void *);

Loop::Loop() :
	Entropy::Tethys::Loop(),
	_loop(),
	_signals(),
	_check_signals(
		chrono::milliseconds(0),
		chrono::milliseconds(100),
		bind(timer, ref(*this), ref(_signals), ref(_check_signals))
	)
{
	uv_loop_init(&_loop);
	Add(_check_signals);
}

Loop::~Loop()
{
	Stop();
	uv_loop_close(&_loop);
}

void Loop::Stop()
{
	uv_stop(&_loop);
}

void Loop::Add(Entropy::Tethys::Task &task)
{
	try
	{
		dynamic_cast<Task &>(task).Added(*this);
	}
	catch(bad_cast &)
	{
		uv_work_t *req = new uv_work_t;
		req->data = &task;

		uv_queue_work(&_loop, req, work, work_after);
	}
}

void Loop::operator () ()
{
	uv_run(&_loop, UV_RUN_DEFAULT);
}

void Loop::setSignal(const int signum, const function<void()> &cb)
{
	if(_signals.find(signum) == _signals.end()) {
		_signals.insert(make_pair(signum, make_shared<Signal>(signum, cb)));

		Add(*_signals.find(signum)->second);
	} else {
		_signals.find(signum)->second->setCallback(cb);
	}
}

uv_loop_t *Loop::Handle()
{
	return &_loop;
}

void work(uv_work_t *req)
{
	auto &t = *static_cast<Entropy::Tethys::Task *>(req->data);

	try
	{
		t();
	}
	catch(exception &e)
	{
		Log << e;
	}
}

void work_after(uv_work_t *req, int status)
{
	if(status != 0)
		ENTROPY_LOG(Log, Severity::Error) << "Work failed: " << uv_strerror(status);

	auto &t = *static_cast<Entropy::Tethys::Task *>(req->data);

	try
	{
		if(t.hasAfter())
			t.After();
	}
	catch(exception &e)
	{
		Log << e;
	}

	delete req;
}

struct data_t {
	bool done;
	Timer *timer;
};

void timer(Loop &loop, map<int, shared_ptr<Signal>> &signals, Timer &timer)
{
	data_t data;

	data.done = true;
	data.timer = &timer;

	uv_walk(loop.Handle(), walk, &data);

	if(data.done) {
		signals.clear();
		timer.Stop();
	}
}

void walk(uv_handle_t *handle, void *d)
{
	data_t *data = reinterpret_cast<data_t *>(d);

	if(handle->type != UV_SIGNAL) {
		if(handle->type != UV_TIMER && handle->data != data->timer) {
			data->done = false;
		}
	}
}

namespace Entropy
{
	namespace Tethys
	{
		namespace UV
		{
			void ThrowIfError(const string &what, const int status)
			{
				if(status < 0) {
					ENTROPY_THROW(
						AttachUvInfo(Exception(what), status)
					);
				}
			}
		}
	}
}
