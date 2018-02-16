/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Stream.hh"
#include <utility>

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Exception;
using Entropy::Tethys::Buffer;

struct write_cb_data {
	Stream *stream;
	Buffer<char> *data;
	uv_buf_t *buffer;
};

void alloc_cb(uv_handle_t *, size_t, uv_buf_t *);
void shutdown_cb(uv_shutdown_t *, int);
void close_cb(uv_handle_t *);
void write_cb(uv_write_t *, int);

Stream::Stream(
	uv_stream_t *h,
	const function<void(Entropy::Tethys::Stream<char> &)> &data,
	const function<void(Stream &)> &eof,
	const function<void(const Entropy::Exception &)> &error
) :
	Entropy::Tethys::Stream<char>(data),
	_handle(h),
	_on_eof(eof),
	_on_error(error)
{}

Stream::~Stream()
{
	try
	{
		ReadStop();
	}
	catch(exception &e)
	{
		Log << e;
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

	try
	{
		ThrowIfError("Failed to Write", uv_write(req, _handle, data->buffer, 1, write_cb));
	}
	catch(...)
	{
		delete data->buffer;
		delete data;
		throw;
	}
}

void Stream::ReadStart()
{
	ThrowIfError("Failed to Start Read", uv_read_start(_handle, alloc_cb, _entropy_tethys_uv_stream_read_cb));
}

void Stream::ReadStop()
{
	uv_shutdown_t *req = new uv_shutdown_t;
	req->data = _handle;

	try
	{
		ThrowIfError("Failed to stop reading stream", uv_read_stop(_handle));
		try
		{
			ThrowIfError("Failed to stop writing stream", uv_shutdown(req, _handle, shutdown_cb));
		}
		catch(Exception &e)
		{
			// 2018-02-13 AMR NOTE: don't throw if "fail" to shutdown because already shutdown
			if(!e.has<SystemErrorCode>() || e.get<SystemErrorCode>().value() != 107)
				throw e;
			else
				delete req;
		}
	}
	catch(...)
	{
		delete req;
		throw;
	}
}

void Stream::EofCb()
{
	ReadStop();
	_on_eof(*this);
}

void Stream::ErrorCb(const Entropy::Exception &e)
{
	_on_error(e);
}

void Stream::ReadCb(const uv_buf_t *buf, const ssize_t nread)
{
	ENTROPY_LOG(Log, Severity::Debug) << nread << " bytes from peer";
	Read(Buffer<char>(nread, buf->len, buf->base));
}

void _entropy_tethys_uv_stream_read_cb(uv_stream_t *handle , ssize_t nread, const uv_buf_t *buf)
{
	Stream *stream = static_cast<Stream *>(handle->data);
	if(nread == UV_EOF) {
		delete [] buf->base;
		stream->EofCb();
	} else if(nread < 0) {
		delete [] buf->base;
		stream->ErrorCb(AttachUvInfo(Exception("Read Failed"), nread));
	} else {
		stream->ReadCb(buf, nread);
	}
}

void write_cb(uv_write_t *req, int status)
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
	// 2018-02-16 AMR TODO: re-use old unfilled buffers
	buf->base = new char[suggested];
	buf->len = suggested;
}

void shutdown_cb(uv_shutdown_t *req, int status)
{
	// 2018-02-13 AMR TODO: is this even remotely kosher
	ThrowIfError("Shutdown failed", status);
	uv_close(reinterpret_cast<uv_handle_t *>(req->data), close_cb);

	delete req;
}

void close_cb(uv_handle_t *)
{}
