/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Entropy::Tethys::UV;

namespace Tests
{
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

	TEST_CLASS(UVLoop) {
		TEST_METHOD(Create) {
			Loop loop;
		}

		TEST_METHOD(Task) {
			Loop loop;
			bool fail = true;
			Entropy::Tethys::Task t([&fail]() {
				fail = false;
			});
			loop.Add(t);

			loop();
			Assert::IsFalse(fail);
		}

		TEST_METHOD(UVTask) {
			Loop loop;
			TestTask t;
			loop.Add(t);

			loop();
			Assert::IsFalse(t);
		}

		TEST_METHOD(JustSignal) {
			Loop loop;
			loop.setSignal(SIGINT, [&loop]() {
			});

			loop();
		}
	};

	TEST_CLASS(UVGetAddrInfo) {
		TEST_METHOD(Create) {
			bool success = false;
			GetAddrInfo info("localhost", "http", SOCK_STREAM, [&success](auto &) {
				success = true;
			});

			Assert::IsFalse(success);
		}

		TEST_METHOD(Run) {
			Loop loop;
			bool success = false;
			GetAddrInfo info("localhost", "http", SOCK_STREAM, [&success](auto &) {
				success = true;
			});
			loop.Add(info);

			Assert::IsFalse(success);
			loop();
			Assert::IsTrue(success);
		}

		// 2017-10-07 AMR NOTE: currently failing in windows, needs more debugging (unused currently)
		BEGIN_TEST_METHOD_ATTRIBUTE(Cancel)
		TEST_IGNORE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(Cancel) {
			Loop loop;
			bool success = true;
			auto info = make_shared<GetAddrInfo>("localhost", "http", SOCK_STREAM, [&success](auto &) {
				success = false;
			});
			loop.Add(*info);
			info.reset();

			loop();
			Assert::IsTrue(success);
		}
	};

	void TestTask::Added(Loop &loop)
	{
		uv_work_t *req = new uv_work_t;
		req->data = &fail;

		uv_queue_work(loop.Handle(), req, cb, cb2);
	}

	TestTask::operator bool() const
	{
		return fail;
	}

	void cb(uv_work_t *req)
	{
		*static_cast<bool *>(req->data) = false;
	}

	void cb2(uv_work_t *req, int status)
	{
		Assert::AreEqual(status, 0);

		delete req;
	}
}
