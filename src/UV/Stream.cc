/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Stream.hh"
#include <utility>

using namespace Entropy::Asio::UV;
using namespace std;

using Entropy::Asio::Exception;
using Entropy::Asio::Buffer;

struct write_cb_data {
	Stream *stream;
	Buffer<char> *data;
	uv_buf_t *buffer;
};

void alloc_cb(uv_handle_t *, size_t, uv_buf_t *);
void shutdown_cb(uv_shutdown_t *, int);
void close_cb(uv_handle_t *);

Stream::Stream(uv_stream_t *h)
	: _handle(h), _buffer(*this)
{}

Stream::~Stream()
{
	try
	{
		uv_shutdown_t *req = new uv_shutdown_t;
		req->data = _handle;

		ThrowIfError("Failed to stop reading stream", uv_read_stop(_handle));
		ThrowIfError("Failed to stop writing stream", uv_shutdown(req, _handle, shutdown_cb));
	}
	catch(Exception &e)
	{
		ENTROPY_LOG(Log, Severity::Error) << "~Stream failed: " << e.what() << " (" << e.get<SystemError>() << ")";
	}
	catch(exception &e)
	{
		ENTROPY_LOG(Log, Severity::Error) << "~Stream failed: " << e.what();
	}
}

void Stream::Write(Buffer<char> &&b)
{
	uv_write_t *req = new uv_write_t;
	write_cb_data *data = new write_cb_data;

	data->stream = this;
	data->data = new Buffer<char>(std::move(b));
	data->buffer = new uv_buf_t;

	data->buffer->base = data->data->data();
	data->buffer->len = data->data->size();

	req->data = data;

	ThrowIfError("Failed to Write", uv_write(req, _handle, data->buffer, 1, _entropy_asio_uv_stream_write_cb));
}

void Stream::ReadStart()
{
	ThrowIfError("Failed to Start Read", uv_read_start(_handle, alloc_cb, _entropy_asio_uv_stream_read_cb));
}

void Stream::ErrorCb(const Exception &e)
{
	throw e;
}

void Stream::ReadCb(const uv_buf_t *buf, const ssize_t nread)
{
	_buffer.AddData(Buffer<char>(buf->len, nread, buf->base));
}

void _entropy_asio_uv_stream_read_cb(uv_stream_t *handle , ssize_t nread, const uv_buf_t *buf)
{
	Stream *stream = static_cast<Stream *>(handle->data);
	if(nread == UV_EOF) {
		stream->DoneCb();
		delete [] buf->base;
	} else if(nread < 0) {
		delete [] buf->base;
		stream->ErrorCb(AttachUvInfo(Exception("Read Failed"), nread));
	} else {
		stream->ReadCb(buf, nread);
	}
}

void _entropy_asio_uv_stream_write_cb(uv_write_t *req, int status)
{
	write_cb_data *data = static_cast<write_cb_data *>(req->data);
	Stream *stream = data->stream;

	delete data->buffer;
	delete data->data;
	delete data;
	delete req;

	if(status < 0) {
		stream->ErrorCb(AttachUvInfo(Exception("Write Failed"), status));
	}
}

void alloc_cb(uv_handle_t *, size_t suggested, uv_buf_t *buf)
{
	buf->base = new char[suggested];
	buf->len = suggested;
}

void shutdown_cb(uv_shutdown_t *req, int status)
{
	// 2017-06-29 AMR NOTE: Stream is reclaimed
	ThrowIfError("Failed to stop writing stream", status); 
	uv_close(reinterpret_cast<uv_handle_t *>(req->data), close_cb);
}

void close_cb(uv_handle_t *handle)
{
	delete handle;
}
