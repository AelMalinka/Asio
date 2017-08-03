/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Task.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

Task::Task()
	: Tethys::Task()
{}

Task::Task(const function<void()> &cb)
	: Tethys::Task(cb)
{}

Task::~Task() = default;
