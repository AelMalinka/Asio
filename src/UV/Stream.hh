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
					public std::basic_iostream<char>,
					public Task
				{
					public:
						Stream(uv_stream_t *);
						virtual ~Stream();
						void Write(Buffer<char> &&);
					protected:
						virtual void ReadStart();
					protected:
						virtual void onData() = 0;
						virtual void onError(const Exception &);
						virtual void onEof() {}
					private:
						void ReadCb(const uv_buf_t *, const ssize_t);
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
