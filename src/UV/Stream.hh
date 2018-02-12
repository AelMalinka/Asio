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
		void _entropy_tethys_uv_stream_write_cb(uv_write_t *, int);
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
						Stream(uv_stream_t *);
						virtual ~Stream();
						virtual void Write(Buffer<char> &&);
						virtual void onData(Tethys::Stream<char> &) = 0;
					protected:
						virtual void ReadStart();
						virtual void ReadStop();
					protected:
						virtual void onError(const Exception &);
						virtual void onEof();
					private:
						void ReadCb(const uv_buf_t *, const ssize_t);
					private:
						uv_stream_t *_handle;
					friend void ::_entropy_tethys_uv_stream_read_cb(uv_stream_t *, ssize_t, const uv_buf_t *);
					friend void ::_entropy_tethys_uv_stream_write_cb(uv_write_t *, int);
				};
			}
		}
	}

#endif
