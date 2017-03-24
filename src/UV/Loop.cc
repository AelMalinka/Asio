/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Loop.hh"

using namespace Entropy::Asio::UV;
using namespace std;

using Entropy::Asio::Log;
using Entropy::Severity;

void work(uv_work_t *);
void work_after(uv_work_t *, int);

Loop::Loop()
	: Entropy::Asio::Loop(), _loop()
{
	uv_loop_init(&_loop);
}

Loop::~Loop()
{
	uv_stop(&_loop);
	while(uv_loop_close(&_loop) == UV_EBUSY);
}

void Loop::Add(Entropy::Asio::Task &task)
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

uv_loop_t *Loop::Handle()
{
	return &_loop;
}

void work(uv_work_t *req)
{
	auto &t = *static_cast<Entropy::Asio::Task *>(req->data);

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

	delete req;
}

namespace Entropy
{
	namespace Asio
	{
		namespace UV
		{
			void ThrowIfError(const string &what, const int status)
			{
				if(status < 0) {
					ENTROPY_THROW(
						Exception(what) <<
						SystemErrorCode(error_code(-status, system_category())) <<
						SystemError(uv_strerror(status))
					);
				}
			}
		}
	}
}
