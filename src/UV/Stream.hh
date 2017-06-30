/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_STREAM_INC
#	define ENTROPY_ASIO_UV_STREAM_INC

#	include "../StreamBuffer.hh"
#	include "Loop.hh"
#	include <functional>
#	include <iostream>

	extern "C" {
		void _entropy_asio_uv_stream_read_cb(uv_stream_t *, ssize_t, const uv_buf_t *);
		void _entropy_asio_uv_stream_write_cb(uv_write_t *, int);
	}

	namespace Entropy
	{
		namespace Asio
		{
			namespace UV
			{
				class Stream :
					public std::iostream,
					public Task
				{
					public:
						explicit Stream(uv_stream_t *);
						virtual ~Stream();
						void Write(Buffer<char> &&);
					protected:
						virtual void ReadStart();
					protected:
						// 2017-06-29 AMR TODO: what are we doing when the read side closes?
						virtual void DoneCb() {}
					protected:
						virtual void ErrorCb(const Exception &);
						// 2017-06-29 AMR NOTE: this is a POSIX type, hopefully defined by libuv
						virtual void ReadCb(const uv_buf_t *, const ssize_t);
					private:
						uv_stream_t *_handle;
						StreamBuffer<Stream, char> _buffer;
					friend void ::_entropy_asio_uv_stream_read_cb(uv_stream_t *, ssize_t, const uv_buf_t *);
					friend void ::_entropy_asio_uv_stream_write_cb(uv_write_t *, int);
				};
			}
		}
	}

#endif
