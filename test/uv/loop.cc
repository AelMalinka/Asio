/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "UV/Loop.cc"
#include <gtest/gtest.h>

using namespace Entropy::Tethys::UV;
using namespace testing;
using namespace std;

namespace {
	void cb(uv_work_t *);
	void cb2(uv_work_t *, int);

	class TestTask :
		public Task
	{
		public:
			void Added(Loop &);
			operator bool() const;
		private:
			bool fail;
	};

	TEST(UVLoop, Create) {
		Loop loop;
	}

	TEST(UVLoop, Task) {
		Loop loop;
		bool fail = true;
		Entropy::Tethys::Task t([&fail]() {
			fail = false;
		});
		loop.Add(t);

		loop();
		EXPECT_FALSE(fail);
	}

	TEST(UVLoop, UVTask) {
		Loop loop;
		TestTask t;
		loop.Add(t);

		loop();
		EXPECT_FALSE(t);
	}

	void TestTask::Added(Loop &loop)
	{
		uv_work_t *req = new uv_work_t;
		req->data = &fail;

		uv_queue_work(loop.Handle(), req, cb, cb2);
	}

	TestTask::operator bool () const
	{
		return fail;
	}

	void cb(uv_work_t *req)
	{
		*static_cast<bool *>(req->data) = false;
	}

	void cb2(uv_work_t *req, int status)
	{
		if(status != 0)
			FAIL() << uv_strerror(status) << endl;

		delete req;
	}
}
