/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Application.hh"

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#ifdef HAVE_LIBUV
#	include "UV/Loop.hh"
#endif

using namespace Entropy::Tethys;
using namespace std;

Application::Application()
	: Entropy::Application(), _loop()
{
	// 2016-10-07 AMR TODO: have build system select default
#	ifdef HAVE_LIBUV
		_loop = make_shared<UV::Loop>();
#	endif
}

Application::Application(const int argc, char *argv[])
	: Entropy::Application(argc, argv), _loop()
{
	// 2016-10-07 AMR TODO: allow runtime specification
#	ifdef HAVE_LIBUV
		_loop = make_shared<UV::Loop>();
#	endif
}

Application::Application(const int argc, char *argv[], const shared_ptr<Loop> &loop)
	: Entropy::Application(argc, argv), _loop(loop)
{}

Application::Application(const int argc, char *argv[], shared_ptr<Loop> &&loop)
	: Entropy::Application(argc, argv), _loop(move(loop))
{}

Application::~Application() = default;

void Application::Stop()
{
	_loop->Stop();
}

void Application::operator () ()
{
	if(!_loop)
		ENTROPY_THROW(Exception("Loop is not instantiated"));

	(*_loop)();
}

void Application::Add(Task &task)
{
	_loop->Add(task);
}

void Application::setSignal(const int signum, const function<void()> &cb)
{
	_loop->setSignal(signum, cb);
}
