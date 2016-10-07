/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Task.hh"

using namespace Entropy::Asio;
using namespace std;

Task::Task() = default;
Task::Task(const Task &) = default;
Task::Task(Task &&) = default;
Task::~Task() = default;

Task::Task(const function<void()> &task)
	: _task(task)
{}

void Task::operator () ()
{
	_task();
}
