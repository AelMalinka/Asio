/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_STREAM_INC
#	define ENTROPY_TETHYS_UV_STREAM_INC

#	include "../Stream.hh"
#	include "Loop.hh"
#	include <functional>
#	include <iostream>

	extern "C" {
		void _entropy_tethys_uv_stream_read_cb(uv_stream_t *, ssize_t, const uv_buf_t *);
	}

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class Stream :
					public Tethys::Stream<char>,
					public Task
				{
					public:
						Stream(
							uv_stream_t *,
							const std::function<void(Tethys::Stream<char> &)> &,
							const std::function<void(Stream &)> &,
							const std::function<void(const Entropy::Exception &)> &
						);
						virtual ~Stream();
						virtual void Write(Buffer<char> &&);
						virtual void EofCb();
						virtual void ErrorCb(const Entropy::Exception &);
					protected:
						virtual void ReadStart();
						virtual void ReadStop();
					private:
						void ReadCb(const uv_buf_t *, const ssize_t);
					private:
						uv_stream_t *_handle;
						std::function<void(Stream &)> _on_eof;
						std::function<void(const Entropy::Exception &)> _on_error;
					friend void ::_entropy_tethys_uv_stream_read_cb(uv_stream_t *, ssize_t, const uv_buf_t *);
				};
			}
		}
	}

#endif
