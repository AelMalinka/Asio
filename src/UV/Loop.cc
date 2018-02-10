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

Loop::Loop() :
	Entropy::Tethys::Loop(),
	_loop(),
	_signals()
{
	uv_loop_init(&_loop);
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
	ENTROPY_LOG(Log, Severity::Debug) << "Setting signal " << signum;

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
	auto *t = static_cast<Entropy::Tethys::Task *>(req->data);

	try
	{
		if(t)
			(*t)();
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

	auto *t = static_cast<Entropy::Tethys::Task *>(req->data);

	try
	{
		if(t && t->hasAfter())
			t->After();
	}
	catch(exception &e)
	{
		Log << e;
	}

	delete req;
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
