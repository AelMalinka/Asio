/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_LOOP_INC
#	define ENTROPY_ASIO_UV_LOOP_INC

#	include "../Loop.hh"
#	include "Task.hh"
#	include <uv.h>

	namespace Entropy
	{
		namespace Asio
		{
			namespace UV
			{
				void ThrowIfError(const std::string &, const int);

				template<typename E>
				typename ::Entropy::detail::WithErrorInfo<E>::type AttachUvInfo(const E &, const int);

				class Loop :
					public Asio::Loop
				{
					public:
						Loop();
						virtual ~Loop();
						virtual void Add(Asio::Task &);
						virtual void operator () ();
						uv_loop_t *Handle();
					private:
						uv_loop_t _loop;
				};
			}
		}
	}

#	include "Loop.impl.hh"

#endif
